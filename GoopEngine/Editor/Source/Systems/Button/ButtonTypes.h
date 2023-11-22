#pragma once
#include <map>
#include <string>
#include <unordered_map>


namespace GE::ECS
{
  enum BUTTON_TYPES{
    CHANGE_SCENE = 0,
    SELECT_CARD,
    DESELECT_CARD,
    END_TURN,
    BUTTON_TYPE_TOTAL
  };

  const std::map<BUTTON_TYPES, std::string> buttonToString
  {
	  { BUTTON_TYPES::CHANGE_SCENE, "Change Scene" },
    { BUTTON_TYPES::SELECT_CARD, "Select Card" },
    { BUTTON_TYPES::DESELECT_CARD, "De-select Card" },
    { BUTTON_TYPES::END_TURN, "End Turn" }

  };

  const std::unordered_map<std::string, BUTTON_TYPES> stringToButton
  {
	  { "Change Scene", BUTTON_TYPES::CHANGE_SCENE },
    { "Select Card", BUTTON_TYPES::SELECT_CARD },
    { "De-select Card", BUTTON_TYPES::DESELECT_CARD },
    { "End Turn", BUTTON_TYPES::END_TURN }
  };
}
