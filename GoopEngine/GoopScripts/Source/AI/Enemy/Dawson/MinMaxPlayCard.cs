/*!************************************************************************
\file IsOutsideAttackRange.cs
\author Han Qin Ding

\brief
C# script attached to a leaf node.
Used to check if the player is outside of the enemy's attack range.
If the player is outside of the enemy's attack range it returns success, else return fails

**************************************************************************/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using GoopScripts.Source.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Source.AI.Enemy.Dawson
{



  internal class MinMaxPlayCard
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;
    private int m_currentSeq = 0;
    //private uint m_maxTurn = 3;
    private bool m_simStarted = false;
    private  bool m_ranAllSeq = false;
    Stats fakePlayer;
    Stats fakeEnemy;
    List<Permutation> permutationList = new List<Permutation>();


    /*!*********************************************************************
  \brief
    Non default constructor of IsOutsideAttackRange class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public MinMaxPlayCard(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
      // Console.WriteLine("Is outside range ID: " + m_nodeID);
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
      
      if(!m_simStarted)
      {
        m_currentSeq = 0;
        Stats playerS = (Stats)Utils.GetScript("Player", "Stats");
        Stats enemyS = (Stats)GetScriptFromID(entityID, "Stats");
        fakePlayer = new Stats(0);
        fakeEnemy = new Stats(0);
        Permutation newP = new Permutation();
        m_ranAllSeq = false;
        m_currentSeq = 0;
        newP.m_turnCount = 0;
        newP.m_playerHealth = playerS.m_healthBar.m_health;
        newP.m_enemyHealth = enemyS.m_healthBar.m_health;
        newP.m_playerDeck = new Deck(playerS.m_deckMngr.m_deck);
        newP.m_enemyDeck = new Deck(playerS.m_deckMngr.m_deck);
        newP.m_playerHand = new List<ValueTuple<CardBase.CardID, uint>>(playerS.m_deckMngr.m_hand); 
        newP.m_enemyHand = new List<ValueTuple<CardBase.CardID, uint>>(enemyS.m_deckMngr.m_hand);
        newP.m_playerDiscard = new List<CardBase.CardID>(playerS.m_deckMngr.m_discard);
        newP.m_enemyDiscard = new List<CardBase.CardID>(enemyS.m_deckMngr.m_discard);
        newP.m_playerBuff = new List<Buff>(playerS.m_buffs.Buffs);
        newP.m_enemyBuff = new List<Buff>(enemyS.m_buffs.Buffs);


        List<int> choices = new List<int>();
        for (int i = 0; i < enemyS.m_deckMngr.m_hand.Count(); ++i)
          choices.Add(i);

        if (enemyS.m_deckMngr.m_hand.Count() >= 3)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            int choiceA = choices[i];
            List<int> remainderA = new List<int>(choices);
            remainderA.RemoveAt(i);
            for (int j = 0; j < remainderA.Count; ++j)
            {
              int choiceB = remainderA[j];
              List<int> remainderB = new List<int>(remainderA);
              remainderB.RemoveAt(j);
              for (int k = 0; k < remainderB.Count; ++k)
              {
                int choiceC = remainderB[k];
                List<int> newChoice = new List<int> { choiceA, choiceB, choiceC };
                newP.m_cardPlayed = newChoice;
                newP.m_cardstoPlay = newChoice;
                permutationList.Add(new Permutation(newP));
              }
            }
          }
        }

        if (enemyS.m_deckMngr.m_hand.Count() >= 2)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            int choiceA = choices[i];
            List<int> remainderA = new List<int>(choices);
            remainderA.RemoveAt(i);
            for (int j = 0; j < remainderA.Count; ++j)
            {
              int choiceB = remainderA[j];
              List<int> newChoice = new List<int> { choiceA, choiceB};
              newP.m_cardPlayed = newChoice;
              newP.m_cardstoPlay = newChoice;
              permutationList.Add(new Permutation(newP));
            }
          }
        }

        if (enemyS.m_deckMngr.m_hand.Count() >= 1)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            List<int> newChoice = new List<int> { choices[i] };
            newP.m_cardPlayed = newChoice;
            newP.m_cardstoPlay = newChoice;
            permutationList.Add(new Permutation(newP));
          }
        }
        m_simStarted = true;
      }
      else
      {
        if(!m_ranAllSeq)
        {
          Permutation currPerm = permutationList[m_currentSeq];
          fakePlayer.m_healthBar.m_health = currPerm.m_playerHealth;
          fakeEnemy.m_healthBar.m_health = currPerm.m_enemyHealth;
          fakePlayer.m_deckMngr.m_deck = currPerm.m_playerDeck;
          fakeEnemy.m_deckMngr.m_deck = currPerm.m_enemyDeck;
          fakePlayer.m_deckMngr.m_hand = currPerm.m_playerHand;
          fakeEnemy.m_deckMngr.m_hand = currPerm.m_enemyHand;
          fakePlayer.m_deckMngr.m_discard = currPerm.m_playerDiscard;
          fakeEnemy.m_deckMngr.m_discard = currPerm.m_enemyDiscard;
          fakePlayer.m_buffs.SetBuff(currPerm.m_playerBuff);
          fakeEnemy.m_buffs.SetBuff(currPerm.m_enemyBuff);


          //check if leah can play attack def attack
          int atkCount = currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_STRIKE) + currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_BEAM);
          int defCount = currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_SHIELD);
          if(atkCount >= 2 && defCount >=1)
          {

          }

          else
          {

          }
        }

        else
        {
          
        }
       
      }



      //List<CardBase.CardID> cards = s.m_playerHand.ToList<CardBase.CardID>();
      //cards.Add(CardBase.CardID.BASIC_ATTACK);
      //cards.Add(CardBase.CardID.SPECIAL_SCREECH);
      //cards.Add(CardBase.CardID.SPECIAL_FLASHBANG);
      //s.m_playerHand = cards.ToArray<CardBase.CardID>();

      //ResetNode();

    }

    /*!*********************************************************************
    \brief
     onFail function for the IsOutsideAttackRange script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the IsOutsideAttackRange script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
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
