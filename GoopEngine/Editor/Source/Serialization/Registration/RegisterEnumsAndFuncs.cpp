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
}

RTTR_REGISTRATION
{
  /* ------------------- FUNCTIONS ------------------- */
  rttr::type::register_converter_func(RttrHelper::dVec2FromString);
  rttr::type::register_converter_func(RttrHelper::dVec3FromString);
  rttr::type::register_converter_func(RttrHelper::Vec2FromString);

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

  rttr::registration::enumeration<GE::ECS::BUTTON_TYPES>("BUTTON_TYPES")
    (
      rttr::value("CHANGE_SCENE", GE::ECS::BUTTON_TYPES::CHANGE_SCENE),
      rttr::value("SELECT_CARD", GE::ECS::BUTTON_TYPES::SELECT_CARD),
      rttr::value("DESELECT_CARD", GE::ECS::BUTTON_TYPES::DESELECT_CARD),
      rttr::value("END_TURN", GE::ECS::BUTTON_TYPES::END_TURN),
      rttr::value("TOTAL_BUTTON_TYPES", GE::ECS::BUTTON_TYPES::BUTTON_TYPE_TOTAL)
      );

  rttr::registration::enumeration<GE::Component::Anchor::AnchorType>("AnchorType")
    (
      rttr::value("IS_ANCHOR", GE::Component::Anchor::AnchorType::IS_ANCHOR),
      rttr::value("IS_ANCHORABLE", GE::Component::Anchor::AnchorType::IS_ANCHORABLE),
      rttr::value("TOTAL_TYPES", GE::Component::Anchor::AnchorType::TOTAL_TYPES)
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
      rttr::value("BASIC_NORMAL_ATTACK", GE::Component::Card::CardID::BASIC_NORMAL_ATTACK),
      rttr::value("BASIC_NORMAL_SHIELD", GE::Component::Card::CardID::BASIC_NORMAL_SHIELD),
      rttr::value("BASIC_NORMAL_BUFF", GE::Component::Card::CardID::BASIC_NORMAL_BUFF),
      rttr::value("PLAYER_BEAM_ATTACK", GE::Component::Card::CardID::PLAYER_BEAM_ATTACK),
      rttr::value("PLAYER_STRIKE_ATTACK", GE::Component::Card::CardID::PLAYER_STRIKE_ATTACK),
      rttr::value("PLAYER_NORMAL_SHIELD", GE::Component::Card::CardID::PLAYER_NORMAL_SHIELD),
      rttr::value("PLAYER_DEBUFF_FLASH_BANG", GE::Component::Card::CardID::PLAYER_DEBUFF_FLASH_BANG),
      rttr::value("PLAYER_BUFF_SMOKESCREEN", GE::Component::Card::CardID::PLAYER_BUFF_SMOKESCREEN),
      rttr::value("PLAYER_BUFF_RAGE", GE::Component::Card::CardID::PLAYER_BUFF_RAGE),
      rttr::value("DAWSON_BEAM_ATTACK", GE::Component::Card::CardID::DAWSON_BEAM_ATTACK),
      rttr::value("DAWSON_STRIKE_ATTACK", GE::Component::Card::CardID::DAWSON_STRIKE_ATTACK),
      rttr::value("DAWSON_NORMAL_SHIELD", GE::Component::Card::CardID::DAWSON_NORMAL_SHIELD),
      rttr::value("DAWSON_BUFF_CHARGE_UP", GE::Component::Card::CardID::DAWSON_BUFF_CHARGE_UP),
      rttr::value("DAWSON_DEBUFF_TIME_WARP", GE::Component::Card::CardID::DAWSON_DEBUFF_TIME_WARP)
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

} // RTTR Registration
