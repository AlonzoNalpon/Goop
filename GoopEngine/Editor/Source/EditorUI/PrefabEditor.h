#pragma once
#include <ImGui/imgui.h>
#include <rttr/type.h>
#include <ObjectFactory/ObjectStructs.h>
#include <Component/Velocity.h>

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

      static bool InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled = false);
      static void InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled = false);
      static void InputList(std::string propertyName, std::deque<GE::Math::dVec3>& list, float fieldWidth, bool disabled = false);
    };
  }
}
