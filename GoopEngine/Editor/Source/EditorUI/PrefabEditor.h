#pragma once
#include <ImGui/imgui.h>
#include <rttr/type.h>
#include <ObjectFactory/ObjectStructs.h>

namespace GE
{
  namespace EditorGUI
  {
    class PrefabEditor
    {
    public:
      static void CreateContent();

    private:
      static ObjectFactory::VariantPrefab m_currPrefab;
      static std::string m_currentFilepath;  // current prefab being edited
      static bool m_isEditing;
    };
  }
}