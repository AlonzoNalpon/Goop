/*!*********************************************************************
\file   RegisterEnumsAndFuncs.cpp
\date   3-November-2023
\brief  This file handles the registration of converter functions  and
        enumerations used in our engine to be recognized by RTTR library.
        Anything that needs to be serialized/deserialized needs to be
        registered.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <ScriptEngine/CSharpStructs.h>

using namespace GE;

// To ignore during serialization, do
//(
//  metadata("NO_SERIALIZE", true)
//)

namespace RttrHelper
{
  GE::Math::Vec2 Vec2FromString(std::string const& str, bool& ok)
  { GE::Math::Vec2 ret{}; ret << str; ok = true; return ret; }

  GE::Math::dVec2 dVec2FromString(std::string const& str, bool& ok)
  { GE::Math::dVec2 ret{}; ret << str; ok = true; return ret; }

  GE::Math::dVec3 dVec3FromString(std::string const& str, bool& ok)
  { GE::Math::dVec3 ret{}; ret << str; ok = true; return ret; }

  std::string StringFromRttrType(rttr::type const& type, bool& ok)
  { return type.get_name().to_string(); }

  rttr::type RttrTypeFromString(std::string const& str, bool& ok)
  { return rttr::type::get_by_name(str); }
}

RTTR_REGISTRATION
{
  /* ------------------- FUNCTIONS ------------------- */
  rttr::type::register_converter_func(RttrHelper::dVec2FromString);
  rttr::type::register_converter_func(RttrHelper::dVec3FromString);
  rttr::type::register_converter_func(RttrHelper::Vec2FromString);
  rttr::type::register_converter_func(RttrHelper::StringFromRttrType);
  rttr::type::register_converter_func(RttrHelper::RttrTypeFromString);

  /* ------------------- ENUMERATIONS ------------------- */
  rttr::registration::enumeration<AI::NODE_TYPE>("NODE_TYPE")
    (
      rttr::value("ROOT_NODE", AI::NODE_TYPE::ROOT_NODE),
      rttr::value("COMPOSITE_NODE", AI::NODE_TYPE::COMPOSITE_NODE),
      rttr::value("LEAF_NODE", AI::NODE_TYPE::LEAF_NODE)
      );

  rttr::registration::enumeration<fMOD::FmodSystem::ChannelType>("ChannelType")
    (
      rttr::value("BGM", fMOD::FmodSystem::ChannelType::BGM),
      rttr::value("SFX", fMOD::FmodSystem::ChannelType::SFX),
      rttr::value("VOICE", fMOD::FmodSystem::ChannelType::VOICE)
      );

  rttr::registration::enumeration<GE::Component::Anchor::AnchorType>("AnchorType")
    (
      rttr::value("IS_ANCHOR", GE::Component::Anchor::AnchorType::IS_ANCHOR),
      rttr::value("IS_ANCHORABLE", GE::Component::Anchor::AnchorType::IS_ANCHORABLE)
      );

  rttr::registration::enumeration<GE::Component::GE_Button::ButtonEventType>("ButtonEventType")
    (
      rttr::value("NO_EVENT", GE::Component::GE_Button::ButtonEventType::NO_EVENT),
      rttr::value("SELECT_CARD", GE::Component::GE_Button::ButtonEventType::SELECT_CARD),
      rttr::value("UNSELECT_CARD", GE::Component::GE_Button::ButtonEventType::UNSELECT_CARD),
      rttr::value("CHANGE_SCENE", GE::Component::GE_Button::ButtonEventType::CHANGE_SCENE),
      rttr::value("UNPAUSE", GE::Component::GE_Button::ButtonEventType::UNPAUSE),
      rttr::value("NEXT_TURN", GE::Component::GE_Button::ButtonEventType::NEXT_TURN),
      rttr::value("QUIT_GAME", GE::Component::GE_Button::ButtonEventType::QUIT_GAME),
      rttr::value("POPUP", GE::Component::GE_Button::ButtonEventType::POPUP),
      rttr::value("TOTAL_EVENTS", GE::Component::GE_Button::ButtonEventType::TOTAL_EVENTS)
      );

  rttr::registration::enumeration<GE::Component::Card::CardID>("CardType")
    (
      rttr::value("NO_CARD", GE::Component::Card::CardID::NO_CARD),
      rttr::value("BASIC_ATTACK", GE::Component::Card::CardID::BASIC_ATTACK),
      rttr::value("BASIC_SHIELD", GE::Component::Card::CardID::BASIC_SHIELD),
      rttr::value("SPECIAL_SCREECH", GE::Component::Card::CardID::SPECIAL_SCREECH),
      rttr::value("LEAH_BEAM", GE::Component::Card::CardID::LEAH_BEAM),
      rttr::value("LEAH_STRIKE", GE::Component::Card::CardID::LEAH_STRIKE),
      rttr::value("LEAH_SHIELD", GE::Component::Card::CardID::LEAH_SHIELD),
      rttr::value("SPECIAL_FLASHBANG", GE::Component::Card::CardID::SPECIAL_FLASHBANG),
      rttr::value("SPECIAL_SMOKESCREEN", GE::Component::Card::CardID::SPECIAL_SMOKESCREEN),
      rttr::value("SPECIAL_RAGE", GE::Component::Card::CardID::SPECIAL_RAGE),
      rttr::value("DAWSON_BEAM", GE::Component::Card::CardID::DAWSON_BEAM),
      rttr::value("DAWSON_SWING", GE::Component::Card::CardID::DAWSON_SWING),
      rttr::value("DAWSON_SHIELD", GE::Component::Card::CardID::DAWSON_SHIELD),
      rttr::value("SPECIAL_CHARGEUP", GE::Component::Card::CardID::SPECIAL_CHARGEUP),
      rttr::value("SPECIAL_TIMEWRAP", GE::Component::Card::CardID::SPECIAL_TIMEWRAP)
      );

  rttr::registration::enumeration<GE::Component::CardHolder::DataType>("DataType")
    (
      rttr::value("QUEUE", GE::Component::CardHolder::DataType::QUEUE),
      rttr::value("DECK", GE::Component::CardHolder::DataType::DECK),
      rttr::value("HAND", GE::Component::CardHolder::DataType::HAND),
      rttr::value("NONE", GE::Component::CardHolder::DataType::NONE)
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
  rttr::registration::enumeration<MONO::CharacterType>("CharacterType")
    (
      rttr::value("PLAYER", MONO::CharacterType::PLAYER),
      rttr::value("BASIC_ENEMY", MONO::CharacterType::BASIC_ENEMY),
      rttr::value("BOSS_P1", MONO::CharacterType::BOSS_P1),
      rttr::value("BOSS_P2", MONO::CharacterType::BOSS_P2)
      );

} // RTTR Registration
