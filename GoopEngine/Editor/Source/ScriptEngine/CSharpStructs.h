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

namespace GE::MONO
{
  struct Deck
  {
    std::vector<Component::Card::CardID> m_cards;
    std::vector<Component::Card::CardID> m_drawOrderDisplay;
  };

  struct DeckManager
  {
    Deck m_deck;
    std::vector<Component::Card::CardID> m_discardDisplay, m_hand, m_queue;
  };
}