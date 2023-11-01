#include <pch.h>
#include "PrefabEditor.h"
#include <filesystem>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>

using namespace GE::EditorGUI;
using namespace ImGui;

ImVec4 const PrefabEditor::m_highlightClr{ 1.f, 1.f, 0.0f, 1.f };
std::string PrefabEditor::m_prefabName{ "Empty" }, PrefabEditor::m_currentFilepath{};
bool PrefabEditor::m_isDragging{ false };

void PrefabEditor::CreateContent()
{
  ImVec2 const region{ GetContentRegionAvail() };
  // Header
  ImGui::Text(("Currently Editing: " + m_prefabName).c_str());

  // receive data from assets browser
  ImGui::InvisibleButton("#InvisibleButton", {region.x * 0.5f, region.y *0.5f});
  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
    {
      // retrieve payload and cast back to base type
      IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
      std::filesystem::path const filepath{ *reinterpret_cast<std::filesystem::path*>(payload->Data) };
      m_currentFilepath = filepath.string();
      m_prefabName = filepath.stem().string();
    }
    ImGui::EndDragDropTarget();
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
