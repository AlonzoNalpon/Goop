/*!*********************************************************************
\file   RegisterClasses.cpp
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

// To ignore during serialization, do
//(
//  metadata("NO_SERIALIZE", true)
//)

namespace RttrHelper
{
  GE::Math::dVec2 dVec2FromString(std::string const& str, bool& ok)
  {
    GE::Math::dVec2 ret{};
    ret << str;
    ok = true;
    return ret;
  }

  GE::Math::dVec3 dVec3FromString(std::string const& str, bool& ok)
  {
    GE::Math::dVec3 ret{};
    ret << str;
    ok = true;
    return ret;
  }
}

RTTR_REGISTRATION
{
  /* ------------------- CLASSES / STRUCTS ------------------- */
  rttr::registration::class_<Math::dVec2>("dVec2")
    .constructor<>()
    .constructor<double, double>()
    .constructor<Math::dVec2 const&>()
    .property("x", &Math::dVec2::x)
    .property("y", &Math::dVec2::y)
    .method("ToString", &Math::dVec2::ToString)
    ;
  
  rttr::registration::class_<Math::Vec2>("Vec2")
    .method("ToString", &Math::Vec2::ToString)
    ;

  rttr::registration::class_<Math::dVec3>("dVec3")
    .constructor<>()
    .constructor<double, double, double>()
    .constructor<Math::dVec3 const&>()
    .property("x", &Math::dVec3::x)
    .property("y", &Math::dVec3::y)
    .property("z", &Math::dVec3::z) 
    .method("ToString", &Math::dVec3::ToString)
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

  /* ------------------- ENUMERATIONS ------------------- */
  rttr::registration::enumeration<AI::NODE_TYPE>("NODE_TYPE")
    (
      rttr::value("ROOT_NODE", AI::NODE_TYPE::ROOT_NODE),
      rttr::value("COMPOSITE_NODE", AI::NODE_TYPE::COMPOSITE_NODE),
      rttr::value("LEAF_NODE", AI::NODE_TYPE::LEAF_NODE),
      rttr::value("NODE_TYPE_COUNT", AI::NODE_TYPE::NODE_TYPE_COUNT)
      );

  rttr::registration::enumeration<fMOD::FmodSystem::ChannelType>("ChannelType")
    (
      rttr::value("BGM", fMOD::FmodSystem::ChannelType::BGM),
      rttr::value("SFX", fMOD::FmodSystem::ChannelType::SFX),
      rttr::value("VOICE", fMOD::FmodSystem::ChannelType::VOICE),
      rttr::value("TOTAL_CHANNELS", fMOD::FmodSystem::ChannelType::TOTAL_CHANNELS)
      );

  rttr::registration::enumeration<GE::Component::Anchor::AnchorType>("AnchorType")
    (
      rttr::value("IS_ANCHOR", GE::Component::Anchor::AnchorType::IS_ANCHOR),
      rttr::value("IS_ANCHORABLE", GE::Component::Anchor::AnchorType::IS_ANCHORABLE),
      rttr::value("TOTAL_TYPES", GE::Component::Anchor::AnchorType::TOTAL_TYPES)
      );

  rttr::registration::enumeration<GE::Component::GE_Button::ButtonEventType>("AnchorType")
    (
      rttr::value("IS_ANCHOR", GE::Component::GE_Button::ButtonEventType::NO_EVENT),
      rttr::value("IS_ANCHORABLE", GE::Component::GE_Button::ButtonEventType::CHANGE_SCENE),
      rttr::value("TOTAL_TYPES", GE::Component::GE_Button::ButtonEventType::TOTAL_EVENTS)
      );

  //rttr::registration::enumeration<ECS::COMPONENT_TYPES>("COMPONENT_TYPES")
  //  (
  //    rttr::value("TRANSFORM", ECS::COMPONENT_TYPES::TRANSFORM),
  //    rttr::value("BOX_COLLIDER", ECS::COMPONENT_TYPES::BOX_COLLIDER),
  //    rttr::value("VELOCITY", ECS::COMPONENT_TYPES::VELOCITY),
  //    rttr::value("SPRITE", ECS::COMPONENT_TYPES::SPRITE),
  //    rttr::value("SPRITE_ANIM", ECS::COMPONENT_TYPES::SPRITE_ANIM),
  //    rttr::value("MODEL", ECS::COMPONENT_TYPES::MODEL),
  //    rttr::value("TWEEN", ECS::COMPONENT_TYPES::TWEEN),
  //    rttr::value("SCRIPTS", ECS::COMPONENT_TYPES::SCRIPTS),
  //    rttr::value("DRAGGABLE", ECS::COMPONENT_TYPES::DRAGGABLE),
  //    rttr::value("ENEMY_AI", ECS::COMPONENT_TYPES::ENEMY_AI),
  //    rttr::value("COMPONENTS_TOTAL", ECS::COMPONENT_TYPES::COMPONENTS_TOTAL)
  //    );

  //rttr::registration::enumeration<ECS::SYSTEM_TYPES>("SYSTEM_TYPES")
  //  (
  //    rttr::value("TRANSFORM", ECS::SYSTEM_TYPES::COLLISION),
  //    rttr::value("PHYSICS", ECS::SYSTEM_TYPES::PHYSICS),
  //    rttr::value("RENDERING", ECS::SYSTEM_TYPES::RENDERING),
  //    rttr::value("SPRITE_ANIM", ECS::SYSTEM_TYPES::SPRITE_ANIM),
  //    rttr::value("DRAGGABLE_OBJECT", ECS::SYSTEM_TYPES::DRAGGABLE_OBJECT),
  //    rttr::value("PLAYER_CONTROLLER", ECS::SYSTEM_TYPES::PLAYER_CONTROLLER),
  //    rttr::value("POST_ROOT_TRANSFORM", ECS::SYSTEM_TYPES::POST_ROOT_TRANSFORM),
  //    rttr::value("PRE_ROOT_TRANSFORM", ECS::SYSTEM_TYPES::PRE_ROOT_TRANSFORM),
  //    rttr::value("ENEMY_SYSTEM", ECS::SYSTEM_TYPES::ENEMY_SYSTEM),
  //    rttr::value("TOTAL_SYSTEMS", ECS::SYSTEM_TYPES::TOTAL_SYSTEMS)
  //    );

  /* ------------------- FUNCTIONS ------------------- */
  rttr::type::register_converter_func(RttrHelper::dVec2FromString);
  rttr::type::register_converter_func(RttrHelper::dVec3FromString);
} // RTTR Registration
