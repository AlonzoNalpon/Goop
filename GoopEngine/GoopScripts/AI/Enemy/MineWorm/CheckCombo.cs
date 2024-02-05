/*!************************************************************************
\file IsOutsideAttackRange.cs
\author Han Qin Ding

\brief
C# script attached to a leaf node.
Used to check if the player is outside of the enemy's attack range.
If the player is outside of the enemy's attack range it returns success, else return fails

**************************************************************************/

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
    Non default constructor of IsOutsideAttackRange class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public CheckCombo(uint currID, uint parentID, uint[] temp, uint size) 
    {
      m_parentID = parentID;
      m_nodeID = currID;
      // //Console.WriteLine("Is outside range ID: " + m_nodeID);
    }


    /*!*********************************************************************
   \brief
     Awake function for the IsOutsideAttackRange script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the IsOutsideAttackRange script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the IsOutsideAttackRange script. This function is called every frame
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
        //Console.WriteLine("Not enough card to Combo");
        OnFail();
      }
      else if(handSize == 2)
      {
        List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };
        if (EnemyStats.m_deckMngr.m_hand.Any(item => specialCards.Contains(item.Item1)))
        {
          //Console.WriteLine("Only has 2 cards in hand, but contains a special card, so we will not play combo");
          OnFail();
        }
        else
        {
          //Console.WriteLine("2 cards to combo, the enemy will combo");
          OnSuccess();
        }
      }
      else
      {
        //Console.WriteLine("Enough cards to combo, the enemy will combo");
        OnSuccess();
      }
      

    }

    /*!*********************************************************************
    \brief
     onFail function for the IsOutsideAttackRange script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the IsOutsideAttackRange script. This function is called when the script succeed.
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
     Update function for the IsOutsideAttackRange script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the IsOutsideAttackRange script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
