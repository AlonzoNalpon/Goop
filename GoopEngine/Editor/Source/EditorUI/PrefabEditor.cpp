#include <pch.h>
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include <Component/Components.h>
#include <Utilities/GoopUtils.h>
#include <EditorUI/AssetBrowser.h>

using namespace GE::EditorGUI;
using namespace ImGui;

GE::ObjectFactory::VariantPrefab PrefabEditor::m_currPrefab;
std::string PrefabEditor::m_currentFilepath{};
bool PrefabEditor::m_isEditing{ false };

void PrefabEditor::CreateContent()
{
  // Header
  std::string const name{ m_isEditing ? m_currPrefab.m_name : "Empty" };
  ImGui::Text(("Currently Editing: " + name).c_str());

  // run drag & drop 
  if (!m_isEditing)
  {
    // receive data from assets browser
    ImGui::InvisibleButton("#InvisibleButton", GetContentRegionAvail());
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_PREFAB"))
      {
        // retrieve payload and cast back to base type
        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
        std::filesystem::path const filepath{ *reinterpret_cast<std::filesystem::path*>(payload->Data) };
        m_currentFilepath = filepath.string();
        m_currPrefab = Serialization::Deserializer::DeserializePrefabToVariant(m_currentFilepath);
        m_isEditing = true;
      }
      ImGui::EndDragDropTarget();
    }
    if (IsItemClicked(ImGuiMouseButton_Right))
    {
      OpenPopup("NewPrefab");
    }
    if (BeginPopup("NewPrefab"))
    {
      if (Selectable("New Prefab"))
      {
        std::string const filename{ AssetBrowser::SaveFileToExplorer("Prefab Files (.pfb)\0.pfb", 1, ".\\Assets\\Prefabs")};
        std::ifstream ifs{ filename };
        if (!ifs)
        {
          GE::Debug::ErrorLogger::GetInstance().LogError("Unable to create " + filename);
          return;
        }
        m_currentFilepath = filename;
        m_currPrefab = ObjectFactory::VariantPrefab();
        std::string::size_type const startPos{ filename.find_last_of('\\') + 1 };
        m_currPrefab.m_name = filename.substr(startPos, filename.find_last_of('.') - startPos);
        m_isEditing = true;
      }
      EndPopup();
    }
  }

  if (!IsWindowFocused())
  {
    ImGui::Text("WINDOW UNFOCUSED! CLICK BACK TO RESUME EDITING!");
    return;
  }

  if (m_isEditing)
  {
    // iterate through components and render UI elements based on type
    for (rttr::variant& component : m_currPrefab.m_components)
    {
      Text(component.get_type().get_name().to_string().c_str());
      ImGui::NewLine();

      if (component.get_type() == rttr::type::get<Component::Sprite>())
      {
        Component::Sprite& sprite = component.get_value<Component::Sprite>();
        ImGui::Columns(2, 0, true);
        ImGui::SetColumnWidth(0, 118.f);
        ImGui::NextColumn();

        unsigned long long val{ sprite.m_spriteData.texture };
        ImageButton(reinterpret_cast<ImTextureID>(val), { 100, 100 }, { 0, 1 }, { 1, 0 });
        if (ImGui::BeginDragDropTarget())
        {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER"))
          {
            if (payload->Data)
            {
              auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
              const char* droppedPath = static_cast<const char*>(payload->Data);
              sprite.m_spriteData.texture = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath));
            }
          }
        }
        ImGui::Columns(1);

        Separator();
        BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
        ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, GetWindowSize().x);

        TableNextColumn();

        auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
        auto const& textureLT{ textureManager.GetTextureLT() };
        if (BeginCombo("##SpriteCombo", textureManager.GetTextureName(sprite.m_spriteData.texture).c_str()))
        {
          for (auto const& it : textureLT)
          {
            if (Selectable(it.first.c_str()))
            {
              auto const& texture{ textureManager.GetTexture(it.second) };
              sprite.m_spriteData.texture = texture.textureHandle;
              sprite.m_spriteData.info.height = texture.height;
              sprite.m_spriteData.info.width = texture.width;
              sprite.m_spriteData.info.texDims = { 1.f, 1.f }; // default
              sprite.m_spriteData.info.texCoords = {}; // bottom left
            }
          }
          EndCombo();
        }
        EndTable();
        Separator();
        // texcoordinates and info you can't edit
        BeginDisabled();
        int imageDims[2]{ static_cast<int>(sprite.m_spriteData.info.width),
                          static_cast<int>(sprite.m_spriteData.info.height) };
        ImGui::InputInt("Image Width", &imageDims[0]);
        ImGui::InputInt("Image Width", &imageDims[1]);
        ImGui::InputFloat2("Tex Coords", &sprite.m_spriteData.info.texCoords.r);
        EndDisabled();
        ImGui::Separator();
        continue;
      }
      else if (component.get_type() == rttr::type::get<Component::SpriteAnim>())
      {
        ImGui::Separator();
        continue;
      }
      else if (component.get_type() == rttr::type::get<Component::Text>())
      {
        if (IsItemClicked(ImGuiMouseButton_Right))
        {
          OpenPopup("RemoveComponent");
        }
        if (BeginPopup("RemoveComponent"))
        {
          if (Selectable("Remove Component"))
          {

          }
          EndPopup();
        }

        Component::Text& textObj = component.get_value<Component::Text>();
        Separator();
        BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
        ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, GetWindowSize().x);

        TableNextColumn();
        ImGui::ColorEdit4("Color", textObj.m_clr.rgba);
        ImGui::DragFloat("Scale", &textObj.m_scale, .001f, 0.f, 5.f);
        ImGui::InputTextMultiline("Text", &textObj.m_text);

        auto const& fontManager{ Graphics::GraphicsEngine::GetInstance().fontManager };
        auto const& fontLT{ fontManager.GetFontLT() }; //lookup table for fonts (string to ID)
        if (BeginCombo("Font", fontManager.GetFontName(textObj.m_fontID).c_str()))
        {
          for (auto const& it : fontLT)
          {
            if (Selectable(it.first.c_str()))
            {
              textObj.m_fontID = it.second;
            }
          }
          EndCombo();
        }
        EndTable();
        Separator();
        continue;
      }
      for (auto& prop : component.get_type().get_properties())
      {
        if (prop.get_name().to_string() == "worldPos") { continue; }

        rttr::type propType{ prop.get_type() };
        std::string const propName{ prop.get_name().to_string() };

        Text(propName.c_str());
        if (propType == rttr::type::get<Math::dVec3>())
        {
          Math::dVec3 value = prop.get_value(component).get_value<Math::dVec3>();
          InputDouble3(propName, value, GetWindowSize().x / 3.f);
          prop.set_value(component, value);
        }
        else if (propType == rttr::type::get<double>())
        {
          double value = prop.get_value(component).get_value<double>();
          ImGui::InputDouble(("##" + propName).c_str(), &value);
          prop.set_value(component, value);
        }
        else if (propType == rttr::type::get<std::string>())
        {
          std::string value = prop.get_value(component).get_value<std::string>();
          if (ImGui::InputText(("##" + propName).c_str(), &value))
          {
            prop.set_value(component, value);
          }
        }
        else if (propType == rttr::type::get<unsigned>() || propType == rttr::type::get<int>() || propType == rttr::type::get<size_t>())
        {
          int value = prop.get_value(component).get_value<int>();
          ImGui::InputInt(("##" + propName).c_str(), &value);
          prop.set_value(component, value);
        }
        else if (propType == rttr::type::get<bool>())
        {
          bool value = prop.get_value(component).get_value<bool>();
          ImGui::Checkbox(("##" + propName).c_str(), &value);
          prop.set_value(component, value);
        }
        else if (propType == rttr::type::get<std::vector<Component::LinearForce>>())
        {
          std::vector<Component::LinearForce> value = prop.get_value(component).get_value<std::vector<Component::LinearForce>>();
          InputList("Forces", value, GetWindowSize().x / 3.f);
          prop.set_value(component, value);
        }
        else if (propType == rttr::type::get<std::deque<Math::dVec3>>())
        {
          std::deque<Math::dVec3> value = prop.get_value(component).get_value<std::deque<Math::dVec3>>();
          InputList("Tween", value, GetWindowSize().x / 3.f);
          prop.set_value(component, value);
        }
      }
      ImGui::Separator();
    }   // end component for loop
    
    Text("Add Component");
    if (BeginCombo("##AddComponent", GE::ECS::componentsToString.at(static_cast<GE::ECS::COMPONENT_TYPES>(0)).c_str()))
    {
      for (int i{}; i < GE::ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
      {
        std::string const compStr{ GE::ECS::componentsToString.at(static_cast<GE::ECS::COMPONENT_TYPES>(i)) };
        if (Selectable(compStr.c_str()))
        {
          std::stringstream ss;
          for (auto const& comp : m_currPrefab.m_components)
          {
            if (compStr == comp.get_type().get_name().to_string())
            {
              ss << "Unable to add component " << comp.get_type().get_name().to_string() << ". Component already exist";
              break;
            }
          }

          if (!ss.str().empty())
          {
            GE::Debug::ErrorLogger::GetInstance().LogError(ss.str());
          }
          else
          {
            rttr::variant ret;
            switch (i)
            {
            case GE::ECS::COMPONENT_TYPES::TRANSFORM:
              ret = Component::Transform();
              break;
            case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
              ret = Component::BoxCollider();
              break;
            case GE::ECS::COMPONENT_TYPES::VELOCITY:
              ret = Component::Velocity();
              break;
            case GE::ECS::COMPONENT_TYPES::SPRITE:
              ret = Component::Sprite();
              break;
            case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
              ret = Component::SpriteAnim();
              break;
            case GE::ECS::COMPONENT_TYPES::MODEL:
              ret = Component::Model();
              break;
            case GE::ECS::COMPONENT_TYPES::TWEEN:
              ret = Component::Tween();
              break;
            case GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
              ret = Component::ScriptHandler();
              break;
            case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
              ret = Component::Draggable();
              break;
            case GE::ECS::COMPONENT_TYPES::TEXT:
              ret = Component::Text();
              break;
            }
            m_currPrefab.m_components.emplace_back(ret);
          }
          break;
        }
      }
      EndCombo();
    }
    Text("Remove Component");
    if (BeginCombo("##RemoveComponent", nullptr))
    {
      for (auto iter{ m_currPrefab.m_components.begin() }; iter != m_currPrefab.m_components.end(); ++iter)
      {
        if (Selectable(iter->get_type().get_name().to_string().c_str()))
        {
          m_currPrefab.m_components.erase(iter);
          break;
        }
      }
      EndCombo();
    }
    ImGui::NewLine();

    if (ImGui::Button("Cancel"))
    {
      if (m_currPrefab.m_components.empty())
      {
        std::remove(m_currentFilepath.c_str());
      }
      m_currPrefab.Clear();
      m_isEditing = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Changes"))
    {
      Serialization::Serializer::GetInstance().SerializeVariantToPrefab(m_currPrefab, m_currentFilepath);
      GE::Debug::ErrorLogger::GetInstance().LogMessage(m_currPrefab.m_name + " successfully saved");
    }
  }
}


bool PrefabEditor::InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled)
{
  bool valChanged{ false };

  BeginDisabled(disabled);
  propertyName = "##" + propertyName;
  TableNextColumn();
  SetNextItemWidth(fieldWidth);
  if (InputDouble((propertyName + "X").c_str(), &property.x, 0, 0, "%.5f")) { valChanged = true; };
  SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Y").c_str(), &property.y, 0, 0, "%.5f")) { valChanged = true; };
  SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Z").c_str(), &property.z, 0, 0, "%.5f")) { valChanged = true; };
  EndDisabled();

  return valChanged;
}

void PrefabEditor::InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled)
{
  int i{};
  for (auto& force : list)
  {
    PushID((std::to_string(i++)).c_str());
    InputDouble3("Force", force.m_magnitude, fieldWidth, disabled);
    InputDouble("Lifetime", &force.m_lifetime);
    InputDouble("Age", &force.m_age);
    Checkbox("IsActive", &force.m_isActive);
    PopID();
  }

  Separator();
  Unindent();
  // 20 magic number cuz the button looks good
  if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
  {
    list.push_back(Component::LinearForce());
  }

  Indent();
}

void PrefabEditor::InputList(std::string propertyName, std::deque<GE::Math::dVec3>& list, float fieldWidth, bool disabled)
{
  int i{};
  for (auto& item : list)
  {
    PushID((std::to_string(i++)).c_str());
    InputDouble3(propertyName + " " + std::to_string(i++), item, fieldWidth, disabled);
    PopID();
  }

  Separator();
  Unindent();
  // 20 magic number cuz the button looks good
  if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
  {
    list.push_back({ 0, 0, 0 });
  }

  Indent();
}