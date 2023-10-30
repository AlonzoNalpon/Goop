#include <pch.h>
#include "PrefabEditor.h"
#include <rttr/type.h>
#include <filesystem>

using namespace GE::EditorGUI;
using namespace ImGui;

ImVec4 const PrefabEditor::m_highlightClr{ 1.f, 1.f, 0.0f, 1.f };
std::string PrefabEditor::m_currentPrefab{ "Empty" }, PrefabEditor::m_currentFilepath{};
bool PrefabEditor::m_isDragging{ false };

void PrefabEditor::CreateContent()
{
  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
    {
      // retrieve payload and cast back to base type
      IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
      std::filesystem::path const filepath{ *reinterpret_cast<std::filesystem::path*>(payload->Data) };
      m_currentFilepath = filepath.string();
      m_currentPrefab = filepath.stem().string();
    }
    ImGui::EndDragDropTarget();
  }



  // Header
  ImGui::Text(("Currently Editing: " + m_currentPrefab).c_str());
  

  ImVec2 const contentRegionSize{ GetWindowContentRegionMax() };

  SetCursorPosX(contentRegionSize.x * 0.5f - ImGui::CalcTextSize("Cancel Save Changes").x * 0.5f);
  SetCursorPosY(contentRegionSize.y * 0.95f - GetTextLineHeight());
  if (ImGui::Button("Cancel"))
  {

  }
  ImGui::SameLine();
  if (ImGui::Button("Save Changes"))
  {
    
  }
  
}
