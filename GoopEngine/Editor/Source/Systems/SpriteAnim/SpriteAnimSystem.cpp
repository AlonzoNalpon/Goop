/*!*********************************************************************
\file   SpriteAnimSystem.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the SpriteAnimSystem
class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <FrameRateController/FrameRateController.h>
#include <Graphics/GraphicsEngine.h>
#include <math.h>

namespace GE::Systems
{
  constexpr double pi = 3.14159265358979323846;

  void SpriteAnimSystem::Start()
  {
    System::Start();
    Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      //Component::Model* model{ m_ecs->GetComponent<Component::Model>(entity) };
      Component::Sprite* sprite{ m_ecs->GetComponent<Component::Sprite>(entity) };
      Component::SpriteAnim* animData{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      if (animData)
      {
        // get the sprite animation
        Graphics::SpriteAnimation const& spriteAnim
        { gEngine.animManager.GetAnim(animData->m_animID) };


        // setting the new sprite based on data
        sprite->m_spriteData.info = spriteAnim.frames[animData->m_currFrame];
      }
    }
  }

  void SpriteAnimSystem::Update()
  {
    Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    double dt{ FPS::FrameRateController::GetInstance().GetDeltaTime() };
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      //Component::Model* model{ m_ecs->GetComponent<Component::Model>(entity) };
      Component::Sprite* sprite{ m_ecs->GetComponent<Component::Sprite>(entity) };
      Component::SpriteAnim* animData{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      // Updating the sprite anim data (if any)
      if (animData)
      {
        // get the sprite animation
        Graphics::SpriteAnimation const& spriteAnim
        { gEngine.animManager.GetAnim(animData->m_animID) };

        // Updating sprite animation data
        {
          // Update the timer and see if frame should be changed
          animData->m_currTime += dt;
          if (animData->m_currTime >= spriteAnim.speed)
          {
            animData->m_currTime -= spriteAnim.speed; // reset speed for next frame
            // move to next frame, or wrap back to start
            u32 const newFrame{ (animData->m_currFrame + 1) };
            animData->m_currFrame = (newFrame >= spriteAnim.frames.size() ?
              0 : newFrame);
          }
        }

        // setting the new sprite based on data
        sprite->m_spriteData.info = spriteAnim.frames[animData->m_currFrame];
      }
    }
  }

  void SpriteAnimSystem::OnDestroyed()
  {
  }

}
