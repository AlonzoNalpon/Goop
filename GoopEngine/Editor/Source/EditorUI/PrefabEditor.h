#pragma once
#include <ImGui/imgui.h>
#include <rttr/type.h>

namespace GE
{
  namespace EditorGUI
  {
    class PrefabEditor
    {
    public:
      static void CreateContent();

    private:
      static ImVec4 const m_highlightClr;
      static std::string m_prefabName, m_currentFilepath;  // current prefab being edited
      static bool m_isEditing;
    };
  }
}
