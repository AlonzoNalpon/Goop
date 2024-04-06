/*!*********************************************************************
\file  CheckCombo.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script for enemyAI Tree.
        Checks if the enemy has enough card to perform a combo

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;
//sing GoopScripts.Player;
using static GoopScripts.Mono.Utils;
using static GoopScripts.Cards.CardBase;
using GoopScripts.Cards;

namespace GoopScripts.AI.Enemy.MineWorm
{

  internal class CheckCombo
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of CheckCombo class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public CheckCombo(uint currID, uint parentID, uint[] temp, uint size) 
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }


    /*!*********************************************************************
    \brief
     Update function for the CheckCombo script. This function is called every frame
     if the script is attached to a leaf node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      Stats EnemyStats = (Stats) GetScriptFromID(entityID, "Stats");
      int handSize = 0;
      foreach (var c in EnemyStats.m_deckMngr.m_hand)
      {
        handSize = (c.Item1!= CardBase.CardID.NO_CARD)? handSize+1 : handSize;
      }

      if(handSize < 2)
      {
        OnFail();
      }
      else if(handSize == 2)
      {
        List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };
        if (EnemyStats.m_deckMngr.m_hand.Any(item => specialCards.Contains(item.Item1)))
        {
          OnFail();
        }
        else
        {
          OnSuccess();
        }
      }
      else
      {
        OnSuccess();
      }
      

    }

    /*!*********************************************************************
    \brief
     onFail function for the CheckCombo script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the CheckCombo script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      JumpToParent();
    }

  }
}
