﻿/*!*********************************************************************
\file   SelectCard.cs
\author chengen.lau\@digipen.edu
\date   27-January-2024
\brief  Script for button events related to cards in the hand. Moves
        the selected card to the queue on click and triggers on hover
        events.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class SelectCard : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    static public uint m_cardHover; // holds entity for hover effect
    Random rng = new Random();

    public SelectCard() { }

    /*!*********************************************************************
		\brief
		  Queues a card when clicked on. Does nothing if the game is in
      resolution phase or if the selected card is not in hand.
    \param entity
      The id of the current entity
		************************************************************************/
    public void OnClick(uint entity)
    {
      if (GameManager.IsResolutionPhase() || GameManager.IsDrawingCard())
      {
        return;
      }

      Stats player = (Stats)Utils.GetScript("Player", "Stats");
      uint cardId = Utils.GetParentEntity(entity);

      if (player.m_deckMngr.IsQueueFull() || !player.m_deckMngr.IsEntityInHand(cardId))
      {
        return;
      }

      Utils.SetIsActiveEntity(m_cardHover, false);
      Utils.PlaySoundF("SFX_CardPlay" + rng.Next(2,4), 0.7f + (float)rng.NextDouble() * 0.2f, Utils.ChannelType.SFX, false);
      player.QueueCardByID(cardId);
      player.m_deckMngr.AlignHandCards();
    }

    /*!*********************************************************************
		\brief
		  Displays the card hover effect by setting the position and active 
      state of entity m_cardHover
    \param entity
      The id of the entity being hovered on
		************************************************************************/
    public void OnHoverEnter(uint entity)
    {
      if (GameManager.IsDrawingCard())
      {
        return;
      }

      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.Z -= 5.0;
      Utils.SetPosition(m_cardHover, pos);
      Utils.SetIsActiveEntity(m_cardHover, true);
    }

    /*!*********************************************************************
		\brief
		  Removes the card hover effect by setting the active state of entity 
      m_cardHover to false
    \param entity
      The id of the entity being hovered on
		************************************************************************/
    public void OnHoverExit(uint entity)
    {
      Utils.SetIsActiveEntity(m_cardHover, false);
    }
  }
}
