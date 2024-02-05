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
using static GoopScripts.Mono.Utils;
using GoopScripts.Cards;
using static GoopScripts.Cards.CardBase;

namespace GoopScripts.AI.Enemy.MineWorm
{

  internal class PerformCombo
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of IsOutsideAttackRange class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public PerformCombo(uint currID, uint parentID, uint[] temp, uint size)
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
      List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };

      //Console.WriteLine("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
      //Console.WriteLine("Start choosing card to perform combo");
      Stats EnemyStats = (Stats)GetScriptFromID(entityID, "Stats");
      Random random = new Random();


      int handSize = 0;
      foreach (var c in EnemyStats.m_deckMngr.m_hand)
      {
        handSize = (c.Item1 != CardBase.CardID.NO_CARD) ? handSize + 1 : handSize;
      }

      int maxComboSize = (handSize >= 3) ? 4 : 2;
      int comboSize = random.Next(2, maxComboSize);
      if (EnemyStats.m_deckMngr.m_hand.Any(item => specialCards.Contains(item.Item1)))
      {
        comboSize = 2;
      }

      List<int> cardsToPlay = new List<int>();

      //Console.WriteLine("HandSize:: " + handSize);
      //Console.WriteLine("Numbers of Cards to Combo: " + comboSize);

      //Console.WriteLine("Enemy Cards in Hand");
      //Console.WriteLine("-----------------------------------------");
      foreach (var c in EnemyStats.m_deckMngr.m_hand)
      {
        if (c.Item1 != CardBase.CardID.NO_CARD)
        {
          //Console.WriteLine(c.ToString());
        }

      }
      //Console.WriteLine("-----------------------------------------\n");

      while (cardsToPlay.Count < comboSize)
      {
        
        int c = random.Next(0, EnemyStats.m_deckMngr.m_hand.Count);
        ////Console.WriteLine("Rand:: " + c + "->" + EnemyStats.m_deckMngr.m_hand[c]);

        if (!cardsToPlay.Contains(c) && !specialCards.Contains(EnemyStats.m_deckMngr.m_hand[c].Item1) && EnemyStats.m_deckMngr.m_hand[c].Item1 != CardBase.CardID.NO_CARD)
        {
          //Console.WriteLine("Added index" + c.ToString() + " Card:" + EnemyStats.m_deckMngr.m_hand[c].Item1);
          cardsToPlay.Add(c);
        }
      }

      //Console.WriteLine("Enemy Cards played");
      //Console.WriteLine("-----------------------------------------");
      int iterNum = 0;
      List<int> orderedNumbers = cardsToPlay.OrderBy(x => x).ToList();
      foreach (int c in orderedNumbers)
      {
        //Console.WriteLine("play card number: " + c);
        //Console.WriteLine(EnemyStats.m_deckMngr.m_hand[c - iterNum].Item1.ToString());
        EnemyStats.QueueCard(c-iterNum);
        //Console.WriteLine("play card done");
        ++iterNum;
      }
      //Console.WriteLine("-----------------------------------------");


      //Console.WriteLine("Perform Combo Finish");
      //Console.WriteLine("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");

      EndAI(entityID);


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
