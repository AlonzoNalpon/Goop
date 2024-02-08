﻿/*!*********************************************************************
\file   Attack.cs
\author w.chinkitbryan\@digipen.edu
\co-author c.phua\@digipen.edu
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
  internal class SpecialChargeUp : CardBase
  {
    /*!*********************************************************************
    \brief
      Overloaded constructor of a card
    \param cardID
      ID of the card 
    ************************************************************************/
    public SpecialChargeUp(CardID cardID, CardType cardType, string animSprite) : base(cardID, cardType, animSprite)
    {
      Value = 2.0f;
      Duration = 2;
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
      source.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 0));
      source.m_buffs.AddBuff(new Buff(Buff.BuffType.FLAT_ATK_UP, 2.0f, 2));
    }
  }
}