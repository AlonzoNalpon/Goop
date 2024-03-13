/*!************************************************************************
\file CheckForSpecialCard.cs
\author Han Qin Ding

\brief
C# script for enemyAi Tree
Checks if the enemy has special card in its hand

**************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;
using static GoopScripts.Mono.Utils;
using static GoopScripts.Cards.CardBase;

namespace GoopScripts.AI.Enemy.MineWorm
{

  internal class CheckForSpecialCard
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of CheckForSpecalCard class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public CheckForSpecialCard(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }


    /*!*********************************************************************
   \brief
     Awake function for the  CheckForSpecialCard script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the  CheckForSpecialCard script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the I CheckForSpecialCard script. This function is called every frame
     if the script is attached to a leaf node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      Stats EnemyStats = (Stats)GetScriptFromID(entityID, "Stats");
      List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };
      if (EnemyStats.m_deckMngr.m_hand.Any(item => specialCards.Contains(item.Item1)))
      {
        OnSuccess();
      }
      else
      {
        OnFail();
      }



    }

    /*!*********************************************************************
    \brief
     onFail function for the  CheckForSpecialCard script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the  CheckForSpecialCard script. This function is called when the script succeed.
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
     Update function for the CheckForSpecialCard script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the  CheckForSpecialCard script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
