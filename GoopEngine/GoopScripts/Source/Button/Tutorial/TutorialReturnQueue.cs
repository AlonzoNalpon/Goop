/*!*********************************************************************
\file   TutorialReturnQueue.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Tutorial script to handle returning of cards from the queue.
        Moves the selected card back to hand on click and triggers
        on hover events.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static GoopScripts.Cards.CardBase;

namespace GoopScripts.Button
{
  public class TutorialReturnQueue : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    /*!*********************************************************************
    \brief
      Spawns the card hover prefab for the player's queue
    \param deltaTime
      The card icon (in queue) entity
    ************************************************************************/
    public void OnHoverEnter(uint entity)
    {
      QueueCardDisplay.ShowCard(entity);
    }

    /*!*********************************************************************
     \brief
       Unqueues a card when clicked on. Does nothing if the game is in
       resolution phase or if the selected card is not in the queue.
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
      uint cardId = Utils.GetParentEntity(entity);
      if (!player.m_deckMngr.IsEntityInQueue(cardId))
      {
        return;
      }

      QueueCardDisplay.DestroyCard();
      Utils.PlaySoundF("SFX_CardPlay1", 1.0f, Utils.ChannelType.SFX, false);
      player.UnqueueCardByID(cardId);
      player.m_deckMngr.AlignHandCards(true);
      Tutorial.m_tutorialToggled = true;
    }

    /*!*********************************************************************
    \brief
      Destroys the card hover prefab instance. This should be called when
      the icon is no longer hovered on
    ************************************************************************/
    public void OnHoverExit(uint entity)
    {
      QueueCardDisplay.DestroyCard();
    }
  }
}
