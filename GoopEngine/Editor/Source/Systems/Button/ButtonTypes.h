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

  // BUTTON_TYPES& operator++(BUTTON_TYPES& rhs) { return rhs = static_cast<ECS::BUTTON_TYPES>((static_cast<unsigned>(rhs) + 1)); }

  // Registration of Components to ECS follows this map
  // Const map so only interated through ranged for,
  // or use the .at(<key>) function
  const std::map<BUTTON_TYPES, std::string> buttonToString
  {
	  { BUTTON_TYPES::CHANGE_SCENE, "Change Scene" },
    { BUTTON_TYPES::SELECT_CARD, "Select Card" },
    { BUTTON_TYPES::DESELECT_CARD, "De-select Card" },
    { BUTTON_TYPES::END_TURN, "End Turn" }

  };

  // Const map so only interated through ranged for,
  // or use the .at(<key>) function
  const std::unordered_map<std::string, BUTTON_TYPES> stringToButton
  {
	  { "Change Scene", BUTTON_TYPES::CHANGE_SCENE },
    { "Select Card", BUTTON_TYPES::SELECT_CARD },
    { "De-select Card", BUTTON_TYPES::DESELECT_CARD },
    { "End Turn", BUTTON_TYPES::END_TURN }
  };

  // std::vector<std::string> ButtonTypesToString(int type) { ; }
  
}
