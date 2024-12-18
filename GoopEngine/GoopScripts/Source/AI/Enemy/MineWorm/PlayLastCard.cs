﻿/*!*********************************************************************
\file   PlayLastCard.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script for enemyAi Tree
        Makes the enemy play the last card in its hand

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;
using static GoopScripts.Mono.Utils;
using static GoopScripts.Cards.CardBase;
using GoopScripts.Cards;

namespace GoopScripts.AI.Enemy.MineWorm
{

  internal class PlayLastCard
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of PlayLastCard class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public PlayLastCard(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }


    /*!*********************************************************************
    \brief
     Update function for the PlayLastCard script. This function is called every frame
     if the script is attached to a leaf node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      Stats EnemyStats = (Stats)GetScriptFromID(entityID, "Stats");
      Random random = new Random();

      int idx = 0;
      for (; idx < EnemyStats.m_deckMngr.m_hand.Count; ++idx)
      {
        if (EnemyStats.m_deckMngr.m_hand[idx].Item1 != CardID.NO_CARD)
        {
          break;
        }
      }

      EnemyStats.QueueCard(idx);
      EndAI(entityID);


    }

    /*!*********************************************************************
    \brief
     onFail function for the PlayLastCard script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the PlayLastCard script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      JumpToParent();
    }

  }
}
