/*!*********************************************************************
\file   PrefabEditor.h
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains the class encapsulating functions for the prefab 
        editor feature. Different windows in the engine reference
        this class to determine which mode is currently running
        (scene or prefab editor).

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include <Singleton/Singleton.h>
#include <Events/Listener.h>
#include <ECS/Entity/Entity.h>
#include <Prefabs/VariantPrefab.h>

namespace GE
{
  namespace EditorGUI
  {
    class PrefabEditor : public Singleton<PrefabEditor>, public Events::IEventListener
    {
    public:

      /*!*********************************************************************
      \brief
        Checks if a prefab is currently being edited
      \return
        True if a prefab is currently being edited and false otherwise
      ************************************************************************/
      static inline bool const& IsEditingPrefab() noexcept { return m_isEditing; }

      /*!*********************************************************************
      \brief
        Returns the entity spawned as a prefab instance in the prefab editor
      \return
        The entity id of the prefab instance being edited
      ************************************************************************/
      static inline ECS::Entity const& GetCurrentEntity() noexcept { return m_prefabInstance; }

      /*!*********************************************************************
      \brief
        Renders the popup when returning back to the scene from the prefab
        editor.
      ************************************************************************/
      static void RenderBackToScenePopup();

      /*!*********************************************************************
      \brief
        This function handles the corresponding events the PrefabEditor
        subscribed to.

        EDIT_PREFAB
          - Switches the editor to Prefab-editing mode

        KEY_TRIGGERED
          - Checks for ESC key input to return back to scene

        DELETE_PREFAB_CHILD
          - Keeps track of any child of the prefab that gets removed

      \param event
        The event to be handled
      ************************************************************************/
      void HandleEvent(Events::Event* event);

    private:
      static std::string m_prefabName, m_prefabPath;
      static std::unordered_map<ECS::Entity, Prefabs::PrefabSubData::SubDataId> m_entityToSubData;  // to track removal of children/components
      static std::vector<Prefabs::PrefabSubData::SubDataId> m_removedChildren;
      static std::vector<std::pair<Prefabs::PrefabSubData::SubDataId, rttr::type>> m_removedComponents;
      static ECS::Entity m_prefabInstance;
      static bool m_newPrefab, m_isEditing, m_escapeTriggered;

      /*!*********************************************************************
      \brief
        This function is called after a prefab has been modified. It compares
        the current set of components with the previous copy and keeps track
        of any component deletions. These components will then be recorded
        to update any existing instances in future
      ************************************************************************/
      static void CheckForDeletions();
      
      /*!*********************************************************************
      \brief
        Clears and resets all variables of the prefab editor
      ************************************************************************/
      static void ResetPrefabEditor();
    };
  }
}
#endif