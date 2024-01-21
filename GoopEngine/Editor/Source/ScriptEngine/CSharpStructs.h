/*!*********************************************************************
\file   CSharpStructs.h
\author chengen.lau\@digipen.edu
\date   19-January-2024
\brief  This file contains the structs used to display and modify C#
        objects on the inspector.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <vector>
#include <Component/Card.h>
#include <ScriptEngine/ScriptInstance.h>

namespace GE::MONO
{
  // should follow order in CharacterType.cs
  const std::vector<std::string> CharacterTypeToString
  {
    "PLAYER", "BASIC_ENEMY", "BOSS_P1", "BOSS_P2"
  };

  enum CharacterType
  {
    PLAYER,
    BASIC_ENEMY,
    BOSS_P1,  // phase 1
    BOSS_P2   // phase 2
  };

  // should mimic Deck.cs
  struct Deck
  {
    std::vector<Component::Card::CardID> m_cards;
    std::vector<Component::Card::CardID> m_drawOrderDisplay;
  };

  // should mimic DeckManager.cs
  struct DeckManager
  {
    Deck m_deck;
    std::vector<Component::Card::CardID> m_discardDisplay, m_hand, m_queue;
    ScriptInstance m_deckManagerInstance, m_deckInstance; //used to cache the mono data of deck and deckmanager
  };


}