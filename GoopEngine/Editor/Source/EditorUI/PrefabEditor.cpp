#include <pch.h>
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>

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
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
      {
        // retrieve payload and cast back to base type
        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
        std::filesystem::path const filepath{ *reinterpret_cast<std::filesystem::path*>(payload->Data) };
        m_currentFilepath = filepath.string();
        //m_currPrefab = Serialization::Deserializer::DeserializePrefab(m_currentFilepath);
        m_isEditing = true;
      }
      ImGui::EndDragDropTarget();
    }
  }

  ImVec2 const contentRegionSize{ GetWindowContentRegionMax() };
  

  if (!m_isEditing)
  {
    SetCursorPosX(contentRegionSize.x * 0.5f - ImGui::CalcTextSize("Cancel Save Changes").x * 0.5f);
    SetCursorPosY(contentRegionSize.y * 0.95f - GetTextLineHeight());
    if (ImGui::Button("Cancel"))
    {
      m_currPrefab.Clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Changes"))
    {

    }
  }
}
