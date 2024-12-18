/*!*********************************************************************
\file   RegisterComponents.cpp
\date   3-November-2023
\brief  This file handles the registration of custom classes used in
        our engine to be recognized by RTTR library. Anything that
        needs to be serialized/deserialized needs to be registered.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>
#include <Systems/Enemy/EnemySystem.h>
#include <Serialization/ProxyScripts.h>

using namespace GE;

RTTR_REGISTRATION
{
  /* ------------------- SYSTEMS ------------------- */
  rttr::registration::class_<Systems::PhysicsSystem>("PhysicsSystem");
  rttr::registration::class_<Systems::CollisionSystem>("CollisionSystem");
  rttr::registration::class_<Systems::ScriptSystem>("ScriptSystem");
  rttr::registration::class_<Systems::RenderSystem>("RenderSystem");
  rttr::registration::class_<Systems::SpriteAnimSystem>("SpriteAnimSystem");
  rttr::registration::class_<Systems::EnemySystem>("EnemySystem");
  rttr::registration::class_<Systems::PreRootTransformSystem>("PreRootTransformSystem");
  rttr::registration::class_<Systems::PostRootTransformSystem>("PostRootTransformSystem");
  rttr::registration::class_<Systems::TextRenderSystem>("TextRenderSystem");
  rttr::registration::class_<Systems::TweenSystem>("TweenSystem");
  rttr::registration::class_<Systems::AudioSystem>("AudioSystem");
  rttr::registration::class_<Systems::ButtonSystem>("ButtonSystem");
  rttr::registration::class_<Systems::GameSystem>("GameSystem");
  rttr::registration::class_<Systems::ButtonScriptSystem>("ButtonScriptSystem");
  rttr::registration::class_<Systems::ParticleSystem>("ParticleSystem");
  rttr::registration::class_<Systems::AnimEventsSystem>("AnimEventsSystem");


  /* ------------------- COMPONENTS ------------------- */
  rttr::registration::class_<Component::Transform>("Transform")
    .constructor<>()
    .property("pos", &Component::Transform::m_pos)
    .property("scale", &Component::Transform::m_scale)
    .property("rot", &Component::Transform::m_rot)
    .property("worldPos", &Component::Transform::m_worldPos)
    .property("worldScale", &Component::Transform::m_worldScale)
    .property("worldRot", &Component::Transform::m_worldRot)
    ;

  rttr::registration::class_<Component::BoxCollider>("BoxCollider")
    .constructor<>()
    .property("center", &Component::BoxCollider::m_center)
    .property("width", &Component::BoxCollider::m_width)
    .property("height", &Component::BoxCollider::m_height)
    .property("min", &Component::BoxCollider::m_min)
    .property("max", &Component::BoxCollider::m_max)
#ifndef IMGUI_DISABLE
    .property("render", &Component::BoxCollider::m_render)
#endif
    ;

  rttr::registration::class_<Component::Velocity>("Velocity")
    .constructor<>()
    .property("vel", &Component::Velocity::m_vel)
    .property("acc", &Component::Velocity::m_acc)
    .property("mass", &Component::Velocity::m_mass)
    .property("gravity", &Component::Velocity::m_gravity)
    .property("dragForce", &Component::Velocity::m_dragForce)
    .property("linear forces", &Component::Velocity::m_linearForces)
    //.property("impulse forces", &Component::Velocity::m_impulseForces)
    ;

  rttr::registration::class_<Component::Sprite>("Sprite")
    .constructor<Graphics::SpriteData const&, std::string, bool>()
    (
      rttr::parameter_names("spriteData", "spriteName", "shouldRender")
    )
    .property("spriteData", &Component::Sprite::m_spriteData)
    .property("spriteName", &Component::Sprite::m_spriteName)
    .property("shouldRender", &Component::Sprite::m_shouldRender)
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .constructor<Graphics::gObjID>()
    //.property("currTime", &Component::SpriteAnim::currTime)
    //.property("name", &Component::SpriteAnim::animID)
    //.property("currFrame", &Component::SpriteAnim::currFrame)
    //.property("flags", &Component::SpriteAnim::flags)
    ;

  rttr::registration::class_<Component::EnemyAI>("EnemyAI")
    .constructor<unsigned>()
    (
      rttr::parameter_names("treeID")
      )
    .property("treeID", &Component::EnemyAI::m_treeID)
    ;

  rttr::registration::class_<Component::Text>("Text")
    .constructor<>()
    .property("text", &Component::Text::m_text)
    .property("clr", &Component::Text::m_clr)
    .property("scale", &Component::Text::m_scale)
    .property("fontID", &Component::Text::m_fontID)
    .property("Alignment", &Component::Text::m_alignment)
    ;

    ;
  rttr::registration::class_<Component::Tween>("Tween")
    .constructor<>()
    .property("tweens", &Component::Tween::m_tweens)
    .property("paused", &Component::Tween::m_paused)
    .property("loop", &Component::Tween::m_loop)
    .property("playing", &Component::Tween::m_playing)
    ;

  rttr::registration::class_<Component::Audio>("Audio")
    .constructor<>()
    .property("sounds", &Component::Audio::m_sounds)
    ;

  rttr::registration::class_<Component::GE_Button>("GE_Button")
    .constructor<>()
    .property("param", &Component::GE_Button::m_param)
    .property("eventType", &Component::GE_Button::m_eventType)
    .property("lastCollided", &Component::GE_Button::m_lastCollided)
    .property("currCollided", &Component::GE_Button::m_currCollided)
    ;

  rttr::registration::class_<Component::Scripts>("Scripts")
    .constructor<Component::Scripts::ScriptInstances const&>()
    .property("scriptList", &Component::Scripts::m_scriptList)
    ;

  rttr::registration::class_<Serialization::ProxyScripts>("ProxyScripts")
    .constructor<rapidjson::Value const&>()
    .property("scriptData", &Serialization::ProxyScripts::m_scriptData)
    ;

  rttr::registration::class_<Component::Emitter>("Emitter")
    .constructor<>()
    .property("playOnStart", &Component::Emitter::m_playOnStart)
    .property("playing", &Component::Emitter::m_playing)
    .property("gravity", &Component::Emitter::m_gravity)
    .property("minForce", &Component::Emitter::m_minVel)
    .property("maxForce", &Component::Emitter::m_maxVel)
    .property("minDrag", &Component::Emitter::m_minDrag)
    .property("maxDrag", &Component::Emitter::m_maxDrag)
    .property("maxLifeTime", &Component::Emitter::m_maxLifeTime)
    .property("minLifeTime", &Component::Emitter::m_minLifeTime)
    .property("particlePerMin", &Component::Emitter::m_particlesPerMin)
    ;

  rttr::registration::class_<Component::AnimEvents>("AnimEvents")
    .constructor<>()
    .property("eventList", &Component::AnimEvents::m_eventList)
    ;
} // RTTR Registration
