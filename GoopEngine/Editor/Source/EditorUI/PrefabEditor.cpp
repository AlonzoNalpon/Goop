/*!*********************************************************************
\file   PrefabEditor.cpp
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains the class encapsulating functions for the prefab editor
        window. Most of the UI elements are similar to how they were
        done on the inspector. Currently supports drag and drop from
        the assets browser and real time adding/removing of components
        as well as creating a new prefab.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include <Component/Components.h>
#include <Utilities/GoopUtils.h>
#include <EditorUI/AssetBrowser.h>
#include <PrefabManager/PrefabManager.h>
#include <rttr/enumeration.h>
// Disable reinterpret to larger size
#pragma warning(disable : 4312)

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

  if (!IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
  {
    ImGui::Text("WINDOW UNFOCUSED! CLICK BACK TO RESUME EDITING!");
    return;
  }

  if (m_isEditing)
  {
    // iterate through components and render UI elements based on type
    std::vector<rttr::variant>& components{ m_currPrefab.m_components };
    for (rttr::variant& component : components)
    {
      bool hasSpriteAnim{ false };
      for (rttr::variant& i : components) {
        if (i.get_type().get_wrapped_type() == rttr::type::get<Component::SpriteAnim*>())
        {
          hasSpriteAnim = true; break;
        }
      }

      rttr::type const compType{ component.get_type().get_wrapped_type().get_raw_type() };
      Text(compType.get_name().to_string().c_str());
      ImGui::Separator();

      if (compType == rttr::type::get<Component::Sprite>())
      {
        Component::Sprite& sprite = *component.get_value<Component::Sprite*>();

        ImVec2 const imgSize{ 100, 100 };
        SetCursorPosX(GetContentRegionAvail().x / 2 - imgSize.x / 2);
        ImageButton(reinterpret_cast<ImTextureID>(sprite.m_spriteData.texture), imgSize, { 0, 1 }, { 1, 0 });
        if (ImGui::BeginDragDropTarget())
        {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_IMAGE"))
          {
            if (payload->Data)
            {
              auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
              const char* droppedPath = static_cast<const char*>(payload->Data);
              sprite = Component::Sprite(texManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath)));
            }
          }
        }

        if (hasSpriteAnim) // If there's a sprite anim component, we shouldn't be able to edit
        {
          TextColored({ 1.f, 0.f, 0.f, 1.f }, "SpriteAnim detected! Unable to edit!");
          BeginDisabled();
        }

        Separator();
        /*BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
        ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, GetWindowSize().x);

        TableNextColumn();*/

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
              sprite.m_spriteName = textureManager.GetTextureName(sprite.m_spriteData.texture);
              sprite.m_spriteData.info.height = texture.height;
              sprite.m_spriteData.info.width = texture.width;
              sprite.m_spriteData.info.texDims = { 1.f, 1.f }; // default
              sprite.m_spriteData.info.texCoords = {}; // bottom left
            }
          }
          EndCombo();
        }
       
        int imageDims[2]{ static_cast<int>(sprite.m_spriteData.info.width),
                            static_cast<int>(sprite.m_spriteData.info.height) };
        if (ImGui::InputInt("Image Width", &imageDims[0]))
          imageDims[0] = imageDims[0] < 0 ? 0 : imageDims[0];
        if (ImGui::InputInt("Image Height", &imageDims[1]))
          imageDims[1] = imageDims[1] < 0 ? 0 : imageDims[1];
        sprite.m_spriteData.info.width = static_cast<GLuint>(imageDims[0]);
        sprite.m_spriteData.info.height = static_cast<GLuint>(imageDims[1]);
        ImGui::InputFloat2("Tex Coords", &sprite.m_spriteData.info.texCoords.r);
        ImGui::InputFloat2("Tex Dims", &sprite.m_spriteData.info.texDims.x);
        if (ImGui::Button("Reset"))
        {
          auto const& texture{ textureManager.GetTexture(sprite.m_spriteData.texture) };
          // Name's not necessary. Remove in the future if still not needed.
          sprite.m_spriteName = textureManager.GetTextureName(sprite.m_spriteData.texture);
          sprite.m_spriteData.texture = texture.textureHandle;
          sprite.m_spriteName = textureManager.GetTextureName(sprite.m_spriteData.texture);
          sprite.m_spriteData.info.height = texture.height;
          sprite.m_spriteData.info.width = texture.width;
          sprite.m_spriteData.info.texDims = { 1.f, 1.f }; // default
          sprite.m_spriteData.info.texCoords = {}; // bottom left
        }
        SameLine();
        if (ImGui::Button("Force Width To Match Tex AR"))
        {
          auto const& texture{ textureManager.GetTexture(sprite.m_spriteData.texture) };
          double ar = static_cast<double>(texture.width) / texture.height;
          sprite.m_spriteData.info.width = static_cast<GLint>(sprite.m_spriteData.info.height * ar);
        }

        if (hasSpriteAnim)
          EndDisabled();
        //EndTable();
        ImGui::Separator();
        continue;
      }
      else if (compType == rttr::type::get<Component::SpriteAnim>())
      {
        Component::SpriteAnim& spriteAnimObj = *component.get_value<Component::SpriteAnim*>();

        /*BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
        ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, GetWindowSize().x);
        TableNextColumn();*/

        auto const& animManager{ Graphics::GraphicsEngine::GetInstance().animManager };
        auto const& textureLT{ animManager.GetAnimLT() };
        if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj.animID).c_str()))
        {
          for (auto const& it : textureLT)
          {
            if (Selectable(it.first.c_str()))
            {
              Component::Sprite* spriteObj{ nullptr };
              for (rttr::variant& i : components)
              {
                if (i.get_type().get_wrapped_type() == rttr::type::get<Component::Sprite*>())
                {
                  spriteObj = i.get_value<Component::Sprite*>();
                  break;
                }
              }
              auto const& anim = animManager.GetAnim(it.second);
              spriteAnimObj.animID = it.second;
              spriteAnimObj.currFrame = 0;
              spriteAnimObj.flags = animManager.GetAnim(spriteAnimObj.animID).flags;
              spriteAnimObj.flags &= ~Graphics::SPRITE_ANIM_FLAGS::FINISHED; // we're not finished anymore
              spriteAnimObj.currTime = 0.0;

              // setting correct attributes for sprite
              auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };

              // Set sprite name and the texture handle. This is for rendering in editor
              spriteObj->m_spriteName = textureManager.GetTextureName(anim.texture);
              spriteObj->m_spriteData.texture = anim.texture;
              spriteObj->m_spriteData.info = anim.frames[0]; // and set actual sprite info
            }
          }
          EndCombo();
        }
        // Display animation ID for users to know
        TextColored({ 1.f, .7333f, 0.f, 1.f }, ("Animation ID: " + std::to_string(spriteAnimObj.animID)).c_str());
        //EndTable();
        ImGui::Separator();
        continue;
      }
      else if (compType == rttr::type::get<Component::Text>())
      {
        Component::Text& textObj = *component.get_value<Component::Text*>();

        /*BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
        ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, GetWindowSize().x);

        TableNextColumn();*/
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
        //EndTable();
        ImGui::Separator();
        continue;
      }
      else if (compType == rttr::type::get<Component::GE_Button>())
      {
        Component::GE_Button& button = *component.get_value<Component::GE_Button*>();

        rttr::type const type{ rttr::type::get<GE::Component::GE_Button::ButtonEventType>() };
        if (BeginCombo("Event", type.get_enumeration().value_to_name(button.m_eventType).to_string().c_str()))
        {
          for (Component::GE_Button::ButtonEventType currType{}; currType != Component::GE_Button::ButtonEventType::TOTAL_EVENTS;)
          {
            // get the string ...
            std::string str = type.get_enumeration().value_to_name(currType).to_string().c_str();

            if (Selectable(str.c_str(), currType == button.m_eventType))
            {
              button.m_eventType = currType; // set the current type if selected 
            }
            // and now iterate through
            currType = static_cast<Component::GE_Button::ButtonEventType>(static_cast<int>(currType) + 1);
          }
          EndCombo();
        }
        InputText("Param", &button.m_param);
        ImGui::Separator();
        continue;
      }

      rttr::instance compInst{ component };
      rttr::instance inst
      {
        compInst.get_type().get_raw_type().is_wrapper() ? compInst.get_wrapped_instance() : compInst
      };
      for (auto& prop : inst.get_type().get_properties())
      {
        if (prop.get_name().to_string() == "worldPos") { continue; }

        rttr::type const propType{ prop.get_type() };
        std::string const propName{ prop.get_name().to_string() };

        ImGui::Text(propName.c_str());
        if (propType == rttr::type::get<Math::dVec3>())
        {
          Math::dVec3 value = prop.get_value(inst).get_value<Math::dVec3>();
          InputDouble3(propName, value, GetWindowSize().x / 3.f);
          prop.set_value(inst, value);
        }
        else if (propType == rttr::type::get<double>())
        {
          double value = prop.get_value(inst).get_value<double>();
          ImGui::InputDouble(("##" + propName).c_str(), &value);
          prop.set_value(inst, value);
        }
        else if (propType == rttr::type::get<std::string>())
        {
          std::string value = prop.get_value(inst).get_value<std::string>();
          if (ImGui::InputText(("##" + propName).c_str(), &value))
          {
            prop.set_value(inst, value);
          }
        }
        else if (propType == rttr::type::get<unsigned>() || propType == rttr::type::get<int>() || propType == rttr::type::get<size_t>())
        {
          int value = prop.get_value(inst).get_value<int>();
          ImGui::InputInt(("##" + propName).c_str(), &value);
          prop.set_value(inst, value);
        }
        else if (propType == rttr::type::get<bool>())
        {
          bool value = prop.get_value(inst).get_value<bool>();
          ImGui::Checkbox(("##" + propName).c_str(), &value);
          prop.set_value(inst, value);
        }
        else if (propType == rttr::type::get<std::vector<Component::LinearForce>>())
        {
          std::vector<Component::LinearForce> value = prop.get_value(inst).get_value<std::vector<Component::LinearForce>>();
          InputList("Forces", value, GetWindowSize().x / 3.f);
          prop.set_value(inst, value);
        }
        else if (propType == rttr::type::get<std::deque<Math::dVec3>>())
        {
          std::deque<Math::dVec3> value = prop.get_value(inst).get_value<std::deque<Math::dVec3>>();
          InputList("Tween", value, GetWindowSize().x / 3.f);
          prop.set_value(inst, value);
        }
      }
      ImGui::Separator();
    }   // end component for loop
    
    ImGui::Text("Add Component");
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
            if (compStr == comp.get_type().get_wrapped_type().get_raw_type().get_name().to_string())
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
              ret = rttr::type::get_by_name("Transform").create();
              break;
            case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
              ret = rttr::type::get_by_name("BoxCollider").create();
              break;
            case GE::ECS::COMPONENT_TYPES::VELOCITY:
              ret = rttr::type::get_by_name("Velocity").create();
              break;
            case GE::ECS::COMPONENT_TYPES::SPRITE:
            {
              Component::Sprite comp{};
              auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
              auto const& textureLT{ textureManager.GetTextureLT() };
              auto textureIt = textureLT.begin(); // iterator for first texture (alphabetically)
              auto const& texture = textureManager.GetTexture(textureIt->second);
              comp.m_spriteName = textureIt->first;
              comp.m_spriteData.texture = textureIt->second;
              comp.m_spriteData.info.height = texture.height;
              comp.m_spriteData.info.width = texture.width;
              comp.m_spriteData.info.texDims = { 1.f, 1.f }; // default
              comp.m_spriteData.info.texCoords = {}; // bottom left
              ret = std::make_shared<Component::Sprite>(comp);
              break;
            }
            case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
              ret = std::make_shared<Component::SpriteAnim>();
              break;
            case GE::ECS::COMPONENT_TYPES::TWEEN:
              ret = rttr::type::get_by_name("Tween").create();
              break;
            //case GE::ECS::COMPONENT_TYPES::SCRIPTS:
            //  ret = rttr::type::get_by_name("Scripts").create();
            //  break;
            case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
              ret = rttr::type::get_by_name("Draggable").create();
              break;
            case GE::ECS::COMPONENT_TYPES::TEXT:
              ret = rttr::type::get_by_name("Text").create();
              break;/*
            case GE::ECS::COMPONENT_TYPES::AUDIO:
              ret = rttr::type::get_by_name("Audio").create();
              break;*/
            case GE::ECS::COMPONENT_TYPES::GE_BUTTON:
              ret = rttr::type::get_by_name("GE_Button").create();
              break;
            default:
              GE::Debug::ErrorLogger::GetInstance().LogMessage("Selected Component Type is not Supported");
              break;
            }
            if (ret.is_valid())
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
        if (Selectable(iter->get_type().get_wrapped_type().get_raw_type().get_name().to_string().c_str()))
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
      ResetPrefabEditor();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Changes"))
    {
      /*bool flag{ false };
      for (rttr::variant& component : m_currPrefab.m_components)
      {
        if (component.get_type() == rttr::type::get<Component::SpriteAnim*>())
        {
          Component::SpriteAnim& spriteAnim = *component.get_value<Component::SpriteAnim*>();
          for (rttr::variant& component2 : m_currPrefab.m_components)
          {
            if (component2.get_type() == rttr::type::get<Component::Sprite>())
            {
              Component::Sprite const& sprite = *component2.get_value<Component::Sprite*>();
              spriteAnim.animID = Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(sprite.m_spriteName);
              flag = true;
              break;
            }
          }
          if (flag) { break; }
        }
      }*/
      // save to file
      ++m_currPrefab.m_version;
      Serialization::Serializer::SerializeVariantToPrefab(m_currPrefab, m_currentFilepath);
      GE::Debug::ErrorLogger::GetInstance().LogMessage(m_currPrefab.m_name + " successfully saved");

      // reload prefab and dispatch event
      ObjectFactory::ObjectFactory::GetInstance().ReloadPrefab(m_currPrefab.m_name);
      Events::EventManager::GetInstance().Dispatch(Events::PrefabSavedEvent(m_currPrefab.m_name));

      ResetPrefabEditor();
    }
  }
}

void PrefabEditor::ResetPrefabEditor()
{
  if (m_currPrefab.m_components.empty())
  {
    std::remove(m_currentFilepath.c_str());
  }
  m_currPrefab.Clear();
  m_currentFilepath.clear();
  m_isEditing = false;
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
#endif