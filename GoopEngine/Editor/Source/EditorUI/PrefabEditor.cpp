#include <pch.h>
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>

using namespace GE::EditorGUI;
using namespace ImGui;

ImVec4 const PrefabEditor::m_highlightClr{ 1.f, 1.f, 0.0f, 1.f };
std::string PrefabEditor::m_prefabName{ "Empty" }, PrefabEditor::m_currentFilepath{};
bool PrefabEditor::m_isEditing{ false };

void PrefabEditor::CreateContent()
{
  // Header
  ImGui::Text(("Currently Editing: " + m_prefabName).c_str());

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
        m_prefabName = filepath.stem().string();
        m_isEditing = true;
      }
      ImGui::EndDragDropTarget();
    }
  }

  ImVec2 const contentRegionSize{ GetWindowContentRegionMax() };

  SetCursorPosX(contentRegionSize.x * 0.5f - ImGui::CalcTextSize("Cancel Save Changes").x * 0.5f);
  SetCursorPosY(contentRegionSize.y * 0.95f - GetTextLineHeight());
  if (ImGui::Button("Cancel"))
  {
    m_prefabName = "Empty";
  }
  ImGui::SameLine();
  if (ImGui::Button("Save Changes"))
  {

  }
  
}
