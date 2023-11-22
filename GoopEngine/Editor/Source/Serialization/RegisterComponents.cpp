/*!*********************************************************************
\file   RegisterClasses2.cpp
\date   3-November-2023
\brief  This file handles the registration of custom classes used in
        our engine to be recognized by RTTR library. Anything that
        needs to be serialized/deserialized needs to be registered.


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>
#include <Systems/Enemy/EnemySystem.h>

using namespace GE;

RTTR_REGISTRATION
{
  rttr::registration::class_<Component::Transform>("Transform")
    .constructor<>()
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
#ifndef NO_IMGUI
    .property("render", &Component::BoxCollider::m_render)
#endif
    ;

  rttr::registration::class_<Component::LinearForce>("LinearForce")
    .constructor<>()
    .property("magnitude", &Component::LinearForce::m_magnitude)
    .property("lifetime", &Component::LinearForce::m_lifetime)
    .property("isActive", &Component::LinearForce::m_isActive)
    .property("age", &Component::LinearForce::m_age)
    ;

  rttr::registration::class_<Component::DragForce>("DragForce")
    .constructor<>()
    .property("magnitude", &Component::DragForce::m_magnitude)
    .property("isActive", &Component::DragForce::m_isActive)
    ;

  rttr::registration::class_<Component::Velocity>("Velocity")
    .constructor<>()
    .property("vel", &Component::Velocity::m_vel)
    .property("acc", &Component::Velocity::m_acc)
    .property("mass", &Component::Velocity::m_mass)
    .property("gravity", &Component::Velocity::m_gravity)
    .property("dragForce", &Component::Velocity::m_dragForce)
    .property("forces", &Component::Velocity::m_forces)
    ;

  rttr::registration::class_<Component::Sprite>("Sprite")
    .constructor<unsigned>()
    .property("spriteName", &Component::Sprite::m_spriteName)
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .constructor<Graphics::gObjID>()
    .property("name", &Component::SpriteAnim::m_animID)
    ;

  rttr::registration::class_<Component::Draggable>("Draggable")
    .constructor<>()
    ;

  rttr::registration::class_<Component::EnemyAI>("EnemyAI")
    //.constructor<>()
    .constructor<unsigned>()
    (
      rttr::parameter_names("treeID")
    )
    .property("treeID", &Component::EnemyAI::m_treeID)
    ;

  rttr::registration::class_<Component::Scripts>("Scripts")
    .constructor<std::vector<std::string> const&, unsigned>()
    .property("entityId", &Component::Scripts::m_entityId)
    .property("scriptMap", &Component::Scripts::m_scriptMap)
    ;

  rttr::registration::class_<Graphics::Colorf>("Colorf")
    .constructor<>()
    .property("r", &Graphics::Colorf::r)
    .property("g", &Graphics::Colorf::g)
    .property("b", &Graphics::Colorf::b)
    .property("a", &Graphics::Colorf::a)
    ;
  rttr::registration::class_<Component::Text>("Text")
    .constructor<>()
    .property("text", &Component::Text::m_text)
    .property("clr", &Component::Text::m_clr)
    .property("scale", &Component::Text::m_scale)
    .property("fontID", &Component::Text::m_fontID)
    ;

  rttr::registration::class_<Component::Tween::Action>("Action")
    .constructor<>()
    .property("target", &Component::Tween::Action::m_target)
    .property("duration", &Component::Tween::Action::m_duration)
    ;
  rttr::registration::class_<Component::Tween>("Tween")
    .constructor<>()
    .property("tweens", &Component::Tween::m_tweens)
    .property("timeTaken", &Component::Tween::m_timeTaken)
    .property("timeElapsed", &Component::Tween::m_timeElapsed)
    .property("originalPos", &Component::Tween::m_originalPos)
    .property("started", &Component::Tween::m_started)
    .property("paused", &Component::Tween::m_paused)
    ;

  rttr::registration::class_<Component::Audio>("Audio")
    .constructor<>()
    .property("name", &Component::Audio::m_name)
    .property("loop", &Component::Audio::m_loop)
    .property("playOnStart", &Component::Audio::m_playOnStart)
    .property("channel", &Component::Audio::channel)
    ;

  rttr::registration::class_<Component::GE_Button>("GE_Button")
    .constructor<>()
    .property("param", &Component::GE_Button::m_param)
    .property("eventType", &Component::GE_Button::m_eventType);
    ;

  rttr::registration::class_<Component::Anchor>("Anchor")
    .constructor<>()
    .property("type", &Component::Anchor::m_type)
    .property("anchored", &Component::Anchor::m_anchored)
    ;

  rttr::registration::class_<Component::Card>("Card")
    .constructor<>()
    .property("cardID", &Component::Card::cardID)
    .property("tgtEntity", &Component::Card::tgtEntity)
    ;

  rttr::registration::class_<Component::CardHolder>("CardHolder")
    .constructor<>()
    .property("elements", &Component::CardHolder::elements)
    ;

  rttr::registration::class_<Component::Game>("Game")
    .constructor<>()
    .property("player", &Component::Game::m_player)
    .property("enemy", &Component::Game::m_enemy)
    .property("pauseMenu", &Component::Game::m_pauseMenu)
    ;
} // RTTR Registration
