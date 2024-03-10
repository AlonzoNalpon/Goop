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
#include <filesystem>

namespace GE::EditorGUI
{
  bool SpriteSheetEditor::m_isToggled{ false }, SpriteSheetEditor::m_loadedThisSession{ false };
  std::vector <std::string> SpriteSheetEditor::m_spriteSheetNames;
  std::vector<Serialization::SpriteData> SpriteSheetEditor::m_loadedData;

  void SpriteSheetEditor::CreateContent(const char* tabName)
  {
    ImGui::Begin(tabName);
    rttr::type const sprDataType{ rttr::type::get<Serialization::SpriteData>() };
    static int const propCount{ static_cast<int>(sprDataType.get_properties().size()) + 1 };

    if (ImGui::BeginTable("##SSTable", propCount, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH))
    {
      float const windowMinX{ ImGui::GetWindowContentRegionMin().x };
      ImGui::TableNextColumn();
      // print header row with all property names
      for (auto& prop : rttr::type::get<Serialization::SpriteData>().get_properties())
      {
        ImGui::Text(prop.get_name().to_string().c_str());
        ImGui::TableNextColumn();
      }
      ImGui::TableNextRow();

      int index{};
      bool removeSelected{ false }, added{ false };
      std::vector<std::string>::iterator selectedIter{ m_spriteSheetNames.begin() };
      unsigned selectedIndex{};
      float const dropdownWidth{ ImGui::CalcTextSize("SS_MineWorm_Shield").x + 10.f };
      for (auto& data : m_loadedData)
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
            ImGui::SetNextItemWidth(dropdownWidth);
            std::string const& originalVal{ propVar.get_value<std::string>() };
            if (ImGui::BeginCombo(("##AnimDropdown" + std::to_string(index)).c_str(), originalVal.c_str()))
            {
              for (auto iter{ m_spriteSheetNames.begin() }; iter != m_spriteSheetNames.end(); ++iter)
              {
                if (ImGui::Selectable(iter->c_str()))
                {
                  // if first time selecting, remove the selected file from the list of names
                  // else replace the filename
                  prop.set_value(data, *iter);
                  data.m_filePath = *iter;
                  if (originalVal.empty())
                  {
                    added = true;
                    selectedIter = iter;
                  }
                  else
                  {
                    *iter = originalVal;
                  }
                }
              }
              ImGui::EndCombo();
            }
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

        ImGui::PushID(index);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.f));
        if (ImGui::Button("Remove"))
        {
          removeSelected = true;
          selectedIndex = index;
        }
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered())
        {
          ImVec2 const itemMin{ ImGui::GetItemRectMin() };
          float const midY{ (ImGui::GetItemRectMax().y + itemMin.y) / 2.f };
          ImVec2 const lineStart{ windowMinX, midY }, lineEnd{ itemMin.x, midY};
          ImGui::GetWindowDrawList()->AddLine(lineStart, lineEnd, IM_COL32(190, 0, 0, 100), ImGui::GetItemRectSize().y);
        }

        ImGui::PopID();
        ++index;
        ImGui::TableNextRow();
      }

      // if an entry was newly added or deleted,
      // update the corresponding container
      if (removeSelected)
      {
        auto iterToErase{ m_loadedData.begin() + selectedIndex };
        // remove the name and data from both containers
        m_spriteSheetNames.erase(std::find(m_spriteSheetNames.begin(), m_spriteSheetNames.end(), iterToErase->m_id));
        m_loadedData.erase(iterToErase);
        removeSelected = false;
      }
      else if (added)
      {
        m_spriteSheetNames.erase(selectedIter);
      }

      ImGui::EndTable();
    }
    
    if (ImGui::Button("Add"))
    {
      m_loadedData.emplace_back();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
      ClearEmptyEntries();
      Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };
      std::string const filePath{ am.GetConfigData<std::string>("Sprite Config") };
      Serialization::Serializer::SerializeAny(filePath, m_loadedData);
      Debug::ErrorLogger::GetInstance().LogMessage("Successfully saved sprite sheet data");
      m_loadedData.clear();
      m_spriteSheetNames.clear();
      m_loadedThisSession = m_isToggled = false;

      am.ReloadAllFiles();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
      m_loadedData.clear();
      LoadData();
    }
    ImGui::End();
  }


  void SpriteSheetEditor::ToggleSpriteSheetEditor()
  {
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
    LoadData();
    
    m_loadedThisSession = true;
  }

  void SpriteSheetEditor::LoadData()
  {
    Assets::AssetManager const& am{ Assets::AssetManager::GetInstance() };
    auto const& spriteSheetData{ am.GetSpriteSheetData() };

    std::vector<std::string> loadedNames{}, allNames{};
    loadedNames.reserve(spriteSheetData.size());
    allNames.reserve(spriteSheetData.size());
    m_loadedData.reserve(spriteSheetData.size());

    // get the loaded sprite data from asset manager
    for (auto const& [name, data] : spriteSheetData)
    {
      m_loadedData.emplace_back(data);
      loadedNames.emplace_back(name);
    }

    // iterate through sprite sheet directory and get all available file names
    std::filesystem::path const sprSheetDir{ am.GetConfigData<std::string>("Spritesheet Dir") };
    std::string const ssPrefix{ am.GetConfigData<std::string>("Spritesheet Prefix") };
    for (const auto& file : std::filesystem::recursive_directory_iterator(sprSheetDir))
    {
      std::string fileName{ file.path().stem().string() };
      if (!file.is_regular_file() || !fileName.starts_with(ssPrefix))
      {
        continue;
      }
      allNames.emplace_back(std::move(fileName));
    }

    // we only want to display the filenames that aren't already in the
    // config file, so we sort both vectors and extract the difference
    std::sort(allNames.begin(), allNames.end());
    std::sort(loadedNames.begin(), loadedNames.end());
    std::set_difference(allNames.begin(), allNames.end(), loadedNames.begin(), loadedNames.end(), std::back_inserter(m_spriteSheetNames));
  }

  void SpriteSheetEditor::ClearEmptyEntries()
  {
    for (auto iter{ m_loadedData.begin() }; iter != m_loadedData.end();)
    {
      if (iter->m_id.empty())
      {
        iter = m_loadedData.erase(iter);
      }
      else
      {
        ++iter;
      }
    }
  }

} // namespace GE::EditorGUI
#endif