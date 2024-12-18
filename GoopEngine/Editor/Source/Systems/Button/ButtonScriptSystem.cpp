/*!*********************************************************************
\file   ButtonScriptSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   16-December-2023
\brief  
  This systems calls the appropriate script of behaviour for the
  appropriate entity
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ButtonScriptSystem.h"
#include <Component/Components.h>

void GE::Systems::ButtonScriptSystem::Start()
{
  GE::ECS::System::Start();
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::MouseTriggeredEvent>(this);
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::MouseReleasedEvent>(this);
}

void GE::Systems::ButtonScriptSystem::Update()
{
  for (GE::ECS::Entity entity : GetUpdatableEntities())
  {
    auto col = m_ecs->GetComponent<GE::Component::BoxCollider>(entity);
    auto btn = m_ecs->GetComponent<GE::Component::GE_Button>(entity);
    auto scripts = m_ecs->GetComponent<GE::Component::Scripts>(entity);

    // Any invalid comp, skip
    if (!col || !btn || !scripts)
    {
      continue;
    }

    MonoMethod* method = nullptr;

    if (btn->m_hadEvent && m_shouldHandleRelease)
    {
      btn->m_hadEvent = false;
      uint32_t gchandle{};
      for (auto script : scripts->m_scriptList)
      {
        gchandle = script.m_gcHandle;
        method = mono_class_get_method_from_name(script.m_scriptClass, "OnRelease", 1);
        if (!method)
        {
          MonoClass* parent = mono_class_get_parent(script.m_scriptClass);
          method = mono_class_get_method_from_name(parent, "OnRelease", 1);
        }
        if (method)
        {
          // run click functon
          void* args{ &entity };
          mono_runtime_invoke(method, mono_gchandle_get_target(gchandle), &args, nullptr);
        }
      }
    }

    if (!col->m_mouseCollided)
    {
      btn->m_currCollided = false;
    }
    else
    {
      btn->m_lastCollided = btn->m_currCollided;
      btn->m_currCollided = true;

      // Process if click event
      if (m_shouldHandleClick)
      {
        btn->m_hadEvent = true;
        uint32_t gchandle{};
        for (auto script : scripts->m_scriptList)
        {
          gchandle = script.m_gcHandle;
          method = mono_class_get_method_from_name(script.m_scriptClass, "OnClick", 1);
          if (!method)
          {
            MonoClass* parent = mono_class_get_parent(script.m_scriptClass);
            method = mono_class_get_method_from_name(parent, "OnClick", 1);
          }
          if(method)
          {
            // run click functon
            void* args{ &entity };
            mono_runtime_invoke(method, mono_gchandle_get_target(gchandle), &args, nullptr);
          }
        }
      }
    }

    // Check if theres a change in state
    if (btn->m_currCollided != btn->m_lastCollided)
    {
      uint32_t gchandle{};
      for (auto script : scripts->m_scriptList)
      {
        gchandle = script.m_gcHandle;
        if (btn->m_currCollided)
        {
          method = mono_class_get_method_from_name(script.m_scriptClass, "OnHoverEnter", 1);
          if (!method)
          {
            MonoClass* parent = mono_class_get_parent(script.m_scriptClass);
            method = mono_class_get_method_from_name(parent, "OnHoverEnter", 1);
          }
        }
        else
        {
          method = mono_class_get_method_from_name(script.m_scriptClass, "OnHoverExit", 1);
          if (!method)
          {
            MonoClass* parent = mono_class_get_parent(script.m_scriptClass);
            method = mono_class_get_method_from_name(parent, "OnHoverExit", 1);
          }
        }

        // If found function in script
        if (method)
        {
          // run hover function
          void* args{ &entity };
          mono_runtime_invoke(method, mono_gchandle_get_target(gchandle), &args, nullptr);
        }
      }
    }

    btn->m_lastCollided = btn->m_currCollided;
  }
  m_shouldHandleClick = false;
  m_shouldHandleRelease = false;
}

void GE::Systems::ButtonScriptSystem::HandleEvent(GE::Events::Event* event)
{
  if (event->GetCategory() == GE::Events::EVENT_TYPE::MOUSE_TRIGGERED)
  {
    m_shouldHandleClick = true;
  }
  else if (event->GetCategory() == GE::Events::EVENT_TYPE::MOUSE_RELEASED)
  {
    m_shouldHandleRelease = true;
  }
}
