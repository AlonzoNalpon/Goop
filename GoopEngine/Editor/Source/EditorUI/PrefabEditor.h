/*!*********************************************************************
\file   PrefabEditor.h
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains the class encapsulating functions for the prefab editor
        feature.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include <Singleton/Singleton.h>
#include <Events/Listener.h>
#include <ECS/Entity/Entity.h>

namespace GE
{
  namespace EditorGUI
  {
    class PrefabEditor : public Singleton<PrefabEditor>, public Events::IEventListener
    {
    public:
      static inline bool const& IsEditingPrefab() noexcept { return m_isEditing; }
      static inline ECS::Entity const& GetCurrentEntity() noexcept { return m_prefabInstance; }

      static void RenderBackToScenePopup();

      void HandleEvent(Events::Event* event);

    private:
      static ECS::Entity m_prefabInstance;
      static std::string m_prefabName, m_prefabPath;
      static bool m_isEditing, m_escapeTriggered;

      static void ResetPrefabEditor();
    };
  }
}
#endif