/*!*********************************************************************
\file   RegisterEnumsAndFuncs.cpp
\date   3-November-2023
\brief  This file handles the registration of converter functions  and
        enumerations used in our engine to be recognized by RTTR library.
        Anything that needs to be serialized/deserialized needs to be
        registered.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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

  rttr::registration::enumeration<MONO::CharacterType>("CharacterType")
    (
      rttr::value("PLAYER", MONO::CharacterType::PLAYER),
      rttr::value("BASIC_ENEMY", MONO::CharacterType::BASIC_ENEMY),
      rttr::value("BOSS_P1", MONO::CharacterType::BOSS_P1),
      rttr::value("BOSS_P2", MONO::CharacterType::BOSS_P2)
      );

  rttr::registration::enumeration<GE::Component::Text::TextAlignment>("TextAlignment")
    (
      rttr::value("CENTER", GE::Component::Text::TextAlignment::CENTER),
      rttr::value("LEFT", GE::Component::Text::TextAlignment::LEFT),
      rttr::value("RIGHT", GE::Component::Text::TextAlignment::RIGHT)
      );
} // RTTR Registration
