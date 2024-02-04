﻿/*!*********************************************************************
\file   Attack.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
Attack card, derived from card base.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;
using static GoopScripts.Gameplay.BuffManager;
using System;
using System.Linq;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  internal class DawsonSwing : CardBase
  {
    /*!*********************************************************************
    \brief
      Overloaded constructor of a card
    \param cardID
      ID of the card 
    ************************************************************************/
    public DawsonSwing(CardID cardID, CardType cardType, string animSprite) : base(cardID, cardType, animSprite)
    {
      Value = 2.0f;
      Duration = 1;
    }

    /*!*********************************************************************
    \brief
      Plays a card with reference from a source and target entity
    \param source
      Reference to a source entity
    \param source
      Reference to a target entity
    ************************************************************************/
    public override void Play(ref Stats source, ref Stats target)
    {
      source.AddAttack((int)Value);
    }
  }
}