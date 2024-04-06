/*!*********************************************************************
\file   SpriteAnimSystem.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the SpriteAnimSystem
        class
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <FrameRateController/FrameRateController.h>
#include <Graphics/GraphicsEngine.h>
#include <math.h>

namespace GE::Systems
{
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
        { gEngine.animManager.GetAnim(animData->animID) };

        // setting the new sprite based on data
        sprite->m_spriteData.info = spriteAnim.frames[animData->currFrame];
      }
    }
  }

  void SpriteAnimSystem::Update()
  {
    auto& frc = GE::FPS::FrameRateController::GetInstance();
    frc.StartSystemTimer();
    Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    double dt{ FPS::FrameRateController::GetInstance().GetDeltaTime() };
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      Component::Sprite* sprite{ m_ecs->GetComponent<Component::Sprite>(entity) };
      Component::SpriteAnim* animData{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      // Updating the sprite anim data (if any)
      if (animData)
      {
        // get the sprite animation
        Graphics::SpriteAnimation const& spriteAnim
        { gEngine.animManager.GetAnim(animData->animID) };

        if (spriteAnim.flags & Graphics::SPRITE_ANIM_FLAGS::FINISHED)
          continue; // we skip those that finished

        // Updating sprite animation data
        {
          // Update the timer and see if frame should be changed
          animData->currTime += dt;
          if (animData->currTime >= spriteAnim.speed)
          {
            animData->currTime -= spriteAnim.speed; // reset speed for next frame
            // move to next frame, or wrap back to start
            u32 const newFrame{ (animData->currFrame + 1) };

            // If we reached the end of the spritesheet
            if (newFrame >= spriteAnim.frames.size())
            {
              // If it's a looping animation
              if (animData->flags & Graphics::SPRITE_ANIM_FLAGS::LOOPING)
                animData->currFrame = 0; // it goes back to 0
              else
                animData->flags |= Graphics::SPRITE_ANIM_FLAGS::FINISHED; // and now set to finished
            }
            else
              animData->currFrame = newFrame;
          }
        }

        // setting the new sprite based on data
        sprite->m_spriteData.info.CopyDataNoColor(spriteAnim.frames[animData->currFrame]);
      }
    }
    frc.EndSystemTimer("Sprite Animation");
  }

  void SpriteAnimSystem::SetAnimation(ECS::Entity entity, size_t animID)
  {
    static GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
    auto* spriteAnim = ecs.GetComponent<Component::SpriteAnim>(entity);
    auto const& animManager = Graphics::GraphicsEngine::GetInstance().animManager;
    NULL_CHECK_RET(spriteAnim);
    auto const& animData = animManager.GetAnim(animID);

    spriteAnim->animID = animID;
    spriteAnim->currFrame = 0;
    spriteAnim->flags = animData.flags;
    spriteAnim->flags &= ~Graphics::SPRITE_ANIM_FLAGS::FINISHED; // we're not finished anymore
    spriteAnim->currTime = 0.0;
    

    auto* sprite = ecs.GetComponent<Component::Sprite>(entity);
    //auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
    //sprite->m_spriteName = textureManager.GetTextureName(animData.texture); //EFFICIENCY  
    sprite->m_spriteData.texture = animData.texture;
    sprite->m_spriteData.info = animData.frames[0]; // load the first frame information
  }

}
