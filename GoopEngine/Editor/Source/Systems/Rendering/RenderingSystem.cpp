/*!*********************************************************************
\file   RenderingSystem.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the RenderingSystem.
ECS related and communicates with Renderer in order to queue render requests
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <FrameRateController/FrameRateController.h>
#include <Graphics/GraphicsEngine.h>
#include <math.h>
namespace GE::Systems
{
  constexpr double pi = 3.14159265358979323846;
  void RenderSystem::LateUpdate()
  {
    Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      // GET ALL THE COMPONENTS
      Component::Model*       model{ m_ecs->GetComponent<Component::Model>(entity) };
      Component::Sprite*      sprite{ m_ecs->GetComponent<Component::Sprite>(entity) };
      Component::Transform*   transform{ m_ecs->GetComponent<Component::Transform>(entity) };
      
      
      // Rendering
      Graphics::Rendering::Renderer& renderer{ gEngine.GetRenderer() };
      // Render the object
      renderer.RenderObject(model->m_mdlID, sprite->m_spriteData,
        transform->m_renderTransform
        );
    }
  }
}