#include <pch.h>
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

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
  }

  if (m_isEditing)
  {
    // iterate through components and render UI elements based on type
    for (rttr::variant& component : m_currPrefab.m_components)
    {
      Text(component.get_type().get_name().to_string().c_str());

      if (component.get_type() == rttr::type::get<Component::Sprite>())
      {
        continue;
        //auto& gEngine = Graphics::GraphicsEngine::GetInstance();
        //Component::Sprite sprite{ component.get_value<Component::Sprite>() };
        //std::string texName{ gEngine.textureManager.GetTextureName(sprite.m_spriteData.texture) };// sprite.m_spriteData.texture = gEngine.textureManager.GetTextureID();
        //Text("filename");
        //ImGui::InputText("##Sprite", &texName);
        //ImGui::SameLine();
        //if (ImGui::Button("Change Texture"))
        //{
        //  try
        //  {
        //    gEngine.
        //    continue;
        //  }
        //  catch (GE::Debug::IExceptionBase&)
        //  {
        //    GE::Debug::ErrorLogger::GetInstance().LogError("Texture does not exist");
        //  }
        //}
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
          ImGui::InputFloat3(("##" + propName).c_str(), reinterpret_cast<float*>(&value));
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
      }
    }   // end component for loop

    /*ImVec2 const contentRegionSize{ GetWindowContentRegionMax() };
    SetCursorPosX(contentRegionSize.x * 0.5f - ImGui::CalcTextSize("Cancel Save Changes").x * 0.5f);
    SetCursorPosY(contentRegionSize.y * 0.95f - GetTextLineHeight());*/
    if (ImGui::Button("Cancel"))
    {
      m_currPrefab.Clear();
      m_isEditing = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Changes"))
    {
      //Serialization::Serializer::GetInstance().SerializeVariantToPrefab(m_currPrefab, m_currentFilepath);
    }
  }
}
