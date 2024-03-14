/*!************************************************************************
\file PerformCombo.cs
\author Han Qin Ding

\brief
C# script for enemyAi Tree
Makes the enemy perform a combo by playing 2-3 cards this turn

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
    Non default constructor of PerformCombo class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public PerformCombo(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }


    /*!*********************************************************************
   \brief
     Awake function for the PerformCombo script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the PerformCombo script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the PerformCombo script. This function is called every frame
     if the script is attached to a leaf node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      List<CardID> specialCards = new List<CardID> { CardID.SPECIAL_SCREECH };
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

      while (cardsToPlay.Count < comboSize)
      {
        
        int c = random.Next(0, EnemyStats.m_deckMngr.m_hand.Count);

        if (!cardsToPlay.Contains(c) && !specialCards.Contains(EnemyStats.m_deckMngr.m_hand[c].Item1) && EnemyStats.m_deckMngr.m_hand[c].Item1 != CardBase.CardID.NO_CARD)
        {
          cardsToPlay.Add(c);
        }
      }

      int iterNum = 0;
      List<int> orderedNumbers = cardsToPlay.OrderBy(x => x).ToList();
      foreach (int c in orderedNumbers)
      {
        EnemyStats.QueueCard(c-iterNum);
        ++iterNum;
      }

      EndAI(entityID);
    }

    /*!*********************************************************************
    \brief
     onFail function for the PerformCombo script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the PerformCombo script. This function is called when the script succeed.
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
     Update function for the PerformCombo script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the PerformCombo script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
