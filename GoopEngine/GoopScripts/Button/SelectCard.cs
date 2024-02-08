/*!*********************************************************************
\file   SelectCard.cs
\author chengen.lau\@digipen.edu
\date   27-January-2024
\brief  Script for button events related to cards in the hand. Moves
        the selected card to the queue on click and triggers on hover
        events.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
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
  public class SelectCard : IButtonClick
  {
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
      if (GameManager.IsResolutionPhase())
      {
        return;
      }

      Stats player = (Stats)Utils.GetScript("Player", "Stats");

      if (player.m_deckMngr.IsQueueFull() || !player.m_deckMngr.IsEntityInHand(entity))
      {
        //Console.WriteLine("Card Not In Hand!");
        return;
      }

      Utils.PlaySoundF("SFX_CardPlay5", 1.0f, Utils.ChannelType.SFX, false);
      player.QueueCardByID(entity);
    }
  }
}
