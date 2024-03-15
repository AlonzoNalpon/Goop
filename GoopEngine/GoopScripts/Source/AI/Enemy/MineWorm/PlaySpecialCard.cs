/*!*********************************************************************
\file   PlaySpecialCard.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script for enemyAi Tree
        Makes the enemy play a special card from its hand

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

  internal class PlaySpecialCard
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of PlaySpecialCard class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public PlaySpecialCard(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }


    /*!*********************************************************************
   \brief
     Awake function for the PlaySpecialCard script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the PlaySpecialCard script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the PlaySpecialCard script. This function is called every frame
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
      List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };
      int pos = 0;
      int cardToPlay = 0;
      foreach (var c in EnemyStats.m_deckMngr.m_hand)
      {
        cardToPlay = (c.Item1 == CardID.SPECIAL_SCREECH) ? pos : cardToPlay;
        ++pos;
      }

      EnemyStats.QueueCard(cardToPlay);
      EndAI(entityID);


    }

    /*!*********************************************************************
    \brief
     onFail function for the PlaySpecialCard script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the PlaySpecialCard script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      JumpToParent();
    }


    public void ReturnFromChild(uint entityID)
    {

    }

    public void OnExit()
    {

    }


    /*!*********************************************************************
    \brief
     Update function for the PlaySpecialCard script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the PlaySpecialCard script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
