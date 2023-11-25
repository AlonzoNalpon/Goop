/*!*********************************************************************
\file   PrefabEditor.h
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains the class encapsulating functions for the prefab editor
        window. Most of the UI elements are similar to how they were
        done on the inspector. Currently supports drag and drop from
        the assets browser and real time adding/removing of components
        as well as creating a new prefab.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef NO_IMGUI
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

      static void ResetPrefabEditor();

      /*!*********************************************************************
      \brief
        Helper function to generate UI for Math::dVec3s.
      \param propertyName
        The name of the property
      \param property
        The Math::dVec3 object
      \param fieldWidth
        The width of each dVec3 component
      \param disabled
        Whether editing is allowed
      \return
        True if the value changed and false otherwise
      ************************************************************************/
      static bool InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled = false);

      /*!*********************************************************************
      \brief
        Helper function to generate UI for forces data member of Velocity
        component
      \param propertyName
        The name of the property
      \param list
        The vector of Linear Forces
      \param fieldWidth
        The width of each field
      \param disabled
        Whether editing is allowed
      ************************************************************************/
      static void InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled = false);

      /*!*********************************************************************
      \brief
        Helper function to generate UI for tweens data member of Tweens
        component
      \param propertyName
        The name of the property
      \param list
        The vector of Math::dVec3s
      \param fieldWidth
        The width of each field
      \param disabled
        Whether editing is allowed
      ************************************************************************/
      static void InputList(std::string propertyName, std::deque<GE::Math::dVec3>& list, float fieldWidth, bool disabled = false);
    };
  }
}
#endif