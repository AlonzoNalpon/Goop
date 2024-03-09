/*!*********************************************************************
\file   SpriteSheetEditor.cpp
\author chengen.lau\@digipen.edu
\date   10-March-2024
\brief  Class responsible for ImGui window for editing sprite sheet
        config data (slices, stacks, speed etc.). It uses RTTR to
        display the values loaded into the asset manager and then
        saving it back to the file afterwards to reload.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "SpriteSheetEditor.h"
#include <ImGui/imgui.h>
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <AssetManager/AssetManager.h>
#include <Graphics/GraphicsEngine.h>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>

namespace GE::EditorGUI
{
  bool SpriteSheetEditor::m_isToggled{ false }, SpriteSheetEditor::m_loadedThisSession{ false };
  std::vector<Serialization::SpriteData> SpriteSheetEditor::m_spriteSheetData;

  void SpriteSheetEditor::CreateContent(const char* tabName)
  {
    ImGui::Begin(tabName);
    rttr::type const sprDataType{ rttr::type::get<Serialization::SpriteData>() };
    static int const propCount{ static_cast<int>(sprDataType.get_properties().size()) };

#pragma region SS_TABLE
    ImGui::BeginTable("##SSTable", propCount, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH);
    ImGui::TableNextColumn();
    // print header row with all property names
    for (auto& prop : rttr::type::get<Serialization::SpriteData>().get_properties())
    {
      ImGui::Text(prop.get_name().to_string().c_str());
      ImGui::TableNextColumn();
    }
    ImGui::TableNextRow();

    int index{};
    for (auto& data : m_spriteSheetData)
    {
      // for each property in the SpriteData, display input based on its type
      ImGui::TableNextColumn();
      for (auto& prop : rttr::type::get<Serialization::SpriteData>().get_properties())
      {
        rttr::variant propVar{ prop.get_value(data) };
        std::string const propName{ prop.get_name() };

        if (propVar.is_type<unsigned>())
        {
          unsigned const step{ 1 };
          unsigned propVal{ propVar.get_value<unsigned>() };
          if (ImGui::InputScalar(("##" + propName + std::to_string(index)).c_str(), ImGuiDataType_U32, &propVal, &step))
          {
            prop.set_value(data, propVal);
          }
        }
        else if (propVar.is_type<double>())
        {
          double propVal{ propVar.get_value<double>() };
          if (ImGui::InputDouble(("##" + propName + std::to_string(index)).c_str(), &propVal, 0.1, 0.0, "%.2f"))
          {
            prop.set_value(data, propVal);
          }
        }
        else if (propName == "id")
        {
          auto const& animManager{ Graphics::GraphicsEngine::GetInstance().animManager };
          auto const& textureLT{ animManager.GetAnimLT() };
          ImGui::SetNextItemWidth(ImGui::CalcTextSize("SS_MineWorm_Shield").x);
          if (ImGui::BeginCombo(("##AnimDropdown" + std::to_string(index)).c_str(), propVar.get_value<std::string>().c_str()))
          {
            for (auto const& it : textureLT)
            {
              if (ImGui::Selectable(it.first.c_str()))
              {
                prop.set_value(data, it.first);
                data.m_filePath = it.first;
              }
            }
            ImGui::EndCombo();
          }
          /*std::string propVal{ propVar.get_value<std::string>() };
          if (ImGui::InputText(("##id" + std::to_string(index)).c_str(), &propVal))
          {
            prop.set_value(data, propVal);
            data.m_filePath = propVal;
          }*/
        }
        else if (propName == "filePath")
        {
          ImGui::BeginDisabled();
          ImGui::InputText(("##filePath" + std::to_string(index)).c_str(), &propVar.get_value<std::string>());
          ImGui::EndDisabled();
        }
        else if (propVar.is_type<std::string>() || propVar.is_type<const char*>())
        {
          std::string propVal{ propVar.get_value<std::string>() };
          if (ImGui::InputText(("##" + propName + std::to_string(index)).c_str(), &propVal))
          {
            prop.set_value(data, propVal);
          }
        }
        else
        {
          Debug::ErrorLogger::GetInstance().LogError("Unknown type in sprite sheet data: " + propVar.get_type().get_name().to_string());
        }
        ImGui::TableNextColumn();
      }
      ++index;
      ImGui::TableNextRow();
    }
    ImGui::EndTable();
#pragma endregion
    
    if (ImGui::Button("Save"))
    {
      std::string const filePath{ Assets::AssetManager::GetInstance().GetConfigData<std::string>("Sprite Config") };
      Serialization::Serializer::SerializeAny(filePath, m_spriteSheetData);
      Debug::ErrorLogger::GetInstance().LogMessage("Successfully saved sprite sheet data");
      m_spriteSheetData.clear();
      m_loadedThisSession = m_isToggled = false;
      // RELOAD HERE
    }
    ImGui::End();
  }


  void SpriteSheetEditor::ToggleSpriteSheetEditor()
  {
    // if turning off, save back to file
    if (m_isToggled)
    {

      m_isToggled = false;
      return;
    }

    // we don't have to load again if it hasn't been saved
    m_isToggled = true;
    if (m_loadedThisSession)
    {
      return;
    }
    // load from file
    auto const& spriteSheetData{ Assets::AssetManager::GetInstance().GetSpriteSheetData() };
    for (auto const& [name, data] : spriteSheetData)
    {
      m_spriteSheetData.emplace_back(data);
    }
    
    m_loadedThisSession = true;
  }
} // namespace GE::EditorGUI
#endif