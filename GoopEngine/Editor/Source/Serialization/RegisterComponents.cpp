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
    .constructor<Math::dVec3 const&, Math::dVec3 const&, Math::dVec3 const&>()
    .property("worldPos", &Component::Transform::m_worldPos)
    .property("worldScale", &Component::Transform::m_worldScale)
    .property("worldRot", &Component::Transform::m_worldRot)
    ;

  rttr::registration::class_<Component::BoxCollider>("BoxCollider")
    .constructor<>()
    .constructor<Math::dVec2 const&, double, double>()
    .property("center", &Component::BoxCollider::m_center)
    .property("width", &Component::BoxCollider::m_width)
    .property("height", &Component::BoxCollider::m_height)
#ifndef NO_IMGUI
    .property("render", &Component::BoxCollider::m_render)
#endif
    ;

  rttr::registration::class_<Component::LinearForce>("LinearForce")
    .constructor<>()
    .constructor<Math::dVec3 const&, double, bool, double>()
    .property("magnitude", &Component::LinearForce::m_magnitude)
    .property("lifetime", &Component::LinearForce::m_lifetime)
    .property("isActive", &Component::LinearForce::m_isActive)
    .property("age", &Component::LinearForce::m_age)
    ;

  rttr::registration::class_<Component::DragForce>("DragForce")
    .constructor<>()
    .constructor<double, bool>()
    .property("magnitude", &Component::DragForce::m_magnitude)
    .property("isActive", &Component::DragForce::m_isActive)
    ;

  rttr::registration::class_<Component::Velocity>("Velocity")
    .constructor<>()
    .constructor<GE::Math::dVec3 const&, GE::Math::dVec3 const&, double, GE::Math::dVec3 const&,
                 Component::DragForce, std::vector<Component::LinearForce>>()
    .property("vel", &Component::Velocity::m_vel)
    .property("acc", &Component::Velocity::m_acc)
    .property("mass", &Component::Velocity::m_mass)
    .property("gravity", &Component::Velocity::m_gravity)
    .property("dragForce", &Component::Velocity::m_dragForce)
    .property("forces", &Component::Velocity::m_forces)
    .method("AddForce", &Component::Velocity::AddForce)
    ;

  rttr::registration::class_<Graphics::SpriteData>("SpriteData")
    .constructor<>()
    .property("texture", &Graphics::SpriteData::texture)
    ;
  rttr::registration::class_<Component::Sprite>("Sprite")
    .property("filename", &Component::Sprite::m_spriteName)  // naming it as filename for now
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .property("name", &Component::SpriteAnim::m_animID)
    ;

  rttr::registration::class_<Component::Model>("Model")
    .property("mdlID", &Component::Model::m_mdlID)
    ;

  rttr::registration::class_<Component::Draggable>("Draggable")
    ;

  //rttr::registration::class_<AI::NodeCache>("NodeCache")
  //  .property("nodeID", &Component::NodeCache::m_nodeID)
  //  .property("childIndex", &Component::NodeCache::m_childIndex)
  //  .property("NodeResult", &Component::NodeCache::m_NodeResult)
  //  ;
  rttr::registration::class_<Component::EnemyAI>("EnemyAI")
    .constructor<unsigned>()
    .property("treeID", &Component::EnemyAI::m_treeID)
    ;


  rttr::registration::class_<AI::NodeTemplate>("NodeTemplate")
    .property("nodeType", &AI::NodeTemplate::m_nodeType)
    .property("parentNode", &AI::NodeTemplate::m_parentNode)
    .property("childrenNode", &AI::NodeTemplate::m_childrenNode)
    .property("scriptName", &AI::NodeTemplate::m_scriptName)
    .property("pos", &AI::NodeTemplate::m_pos)
    ;
  rttr::registration::class_<AI::TreeTemplate>("TreeTemplate")
    .property("tree", &AI::TreeTemplate::m_tree)
    .property("treeName", &AI::TreeTemplate::m_treeName)
    .property("treeTempID", &AI::TreeTemplate::m_treeTempID)
    ;

  rttr::registration::class_<Component::ScriptHandler>("ScriptHandler")
    .constructor<std::vector<std::string> const&, unsigned>()
    .property("scriptMap", &Component::ScriptHandler::m_scriptMap)
    .method("AddScript", &Component::ScriptHandler::AddScript)
    ;

  rttr::registration::class_<Graphics::Colorf>("Colorf")
    .property("r", &Graphics::Colorf::r)
    .property("g", &Graphics::Colorf::g)
    .property("b", &Graphics::Colorf::b)
    .property("a", &Graphics::Colorf::a)
    ;
  rttr::registration::class_<Component::Text>("Text")
    .property("text", &Component::Text::m_text)
    .property("clr", &Component::Text::m_clr)
    .property("scale", &Component::Text::m_scale)
    .property("fontID", &Component::Text::m_fontID)
    ;

  rttr::registration::class_<Component::Tween::Action>("Action")
    .property("target", &Component::Tween::Action::m_target)
    .property("duration", &Component::Tween::Action::m_duration)
    ;
  rttr::registration::class_<Component::Tween>("Tween")
    .constructor<>()
    .property("tweens", &Component::Tween::m_tweens)
    .property("timeTaken", &Component::Tween::m_timeTaken)
    .property("timeElapsed", &Component::Tween::m_timeElapsed)
    .property("originalPos", &Component::Tween::m_originalPos)
    .method("AddTween", &Component::Tween::AddTween)
    ;

  rttr::registration::class_<Component::Audio>("Audio")
    .property("isPlaying", &Component::Audio::m_isPlaying)
    .property("isSFX", &Component::Audio::m_isSFX)
    .property("loop", &Component::Audio::m_loop)
    .property("name", &Component::Audio::m_name)
    .property("stream", &Component::Audio::m_stream)
    .property("play", &Component::Audio::m_play)
    .property("volume", &Component::Audio::m_volume)
    ;

} // RTTR Registration
