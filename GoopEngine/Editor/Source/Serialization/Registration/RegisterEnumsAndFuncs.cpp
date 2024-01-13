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
      rttr::value("BASIC_BUFF_SCREECH", GE::Component::Card::CardID::BASIC_BUFF_SCREECH),
      rttr::value("PLAYER_ATTACK_T1", GE::Component::Card::CardID::PLAYER_ATTACK_T1),
      rttr::value("PLAYER_ATTACK_T2", GE::Component::Card::CardID::PLAYER_ATTACK_T2),
      rttr::value("PLAYER_SHIELD", GE::Component::Card::CardID::PLAYER_SHIELD),
      rttr::value("PLAYER_DEBUFF_FLASH_BANG", GE::Component::Card::CardID::PLAYER_DEBUFF_FLASH_BANG),
      rttr::value("PLAYER_BUFF_SMOKESCREEN", GE::Component::Card::CardID::PLAYER_BUFF_SMOKESCREEN),
      rttr::value("PLAYER_BUFF_RAGE", GE::Component::Card::CardID::PLAYER_BUFF_RAGE),
      rttr::value("DAWSON_ATTACK_T1", GE::Component::Card::CardID::DAWSON_ATTACK_T1),
      rttr::value("DAWSON_ATTACK_T2", GE::Component::Card::CardID::DAWSON_ATTACK_T2),
      rttr::value("DAWSON_SHIELD", GE::Component::Card::CardID::DAWSON_SHIELD),
      rttr::value("DAWSON_BUFF_CHARGE_UP", GE::Component::Card::CardID::DAWSON_BUFF_CHARGE_UP),
      rttr::value("DAWSON_DEBUFF_TIME_WARP", GE::Component::Card::CardID::DAWSON_DEBUFF_TIME_WARP)
      );

} // RTTR Registration
