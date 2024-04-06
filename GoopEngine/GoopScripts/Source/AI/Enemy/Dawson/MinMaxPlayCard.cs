/*!*********************************************************************
\file   MinMaxPlayCard.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  This script handles the AI for enemy Dawson. the script uses
        MinMax AI Algorithm. By running different permutations, the AI
        is able to determine the best course of action

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
//#define PRINT_PERM
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using GoopScripts.Source.Mono;
using static GoopScripts.Cards.CardBase;
using static GoopScripts.Mono.Utils;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;


namespace GoopScripts.Source.AI.Enemy.Dawson
{


  internal class MinMaxPlayCard
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;
    //private int m_currentSeq = 0;
    private int m_maxTurn = 2;
    private int m_maxSeqNum = 90;
    private bool m_simStarted = false;
    //private  bool m_ranAllSeq = false;
    private int m_maxThread = 3;
    private int m_threadDone = 0;
    static Mutex permMutex = new Mutex();
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


    public void SimulatePermutations(int start, int size)
    {
      Stats fakePlayer = new Stats(0);
      Stats fakeEnemy = new Stats(0);
      fakePlayer.FakeOnCreate();
      fakeEnemy.FakeOnCreate();
      int currPermNum = start;
      int end = start + (size);
      while (currPermNum < end)
      {

        //Set the stats for the current Permutation
        Permutation currPerm = permutationList[currPermNum];
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
        while (currPerm.m_turnCount <= (m_maxTurn))
        {
#if PRINT_PERM
          permMutex.WaitOne();
          Console.WriteLine("Before--------------------------------------------------");
          Console.WriteLine("Player Health:  " + fakePlayer.m_healthBar.m_health);
          Console.WriteLine("Player Hand:    " + fakePlayer.m_deckMngr.m_hand.Count);
          Console.WriteLine("Player Discard: " + fakePlayer.m_deckMngr.m_discard.Count);
          Console.WriteLine("Player Buff:    " + fakePlayer.m_buffs.Buffs.Count);
          Console.WriteLine("Enemy Health:   " + fakeEnemy.m_healthBar.m_health);
          Console.WriteLine("Enemy hand:     " + fakeEnemy.m_deckMngr.m_hand.Count);
          Console.WriteLine("Enemy discard:  " + fakeEnemy.m_deckMngr.m_discard.Count);
          Console.WriteLine("Enemy buff:     " + fakeEnemy.m_buffs.Buffs.Count);
          Console.WriteLine("--------------------------------------------------");
          Console.WriteLine("+++++++++++++++++++++++++++++++++++++++");
          permMutex.ReleaseMutex();
#endif
          //Let the player and enemy queue cards
          //check if leah can play attack def attack
          int strikeCount = currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_STRIKE);
          int beamCount = currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_BEAM);
          int defCount = currPerm.m_playerHand.Count(pair => pair.Item1 == CardBase.CardID.LEAH_SHIELD);
          if ((strikeCount + beamCount) >= 2 && defCount >= 1)
          {
            if (strikeCount >= 1)
            {
              fakePlayer.QueueCardByCardID(CardBase.CardID.LEAH_STRIKE);

              --strikeCount;
            }
            else
            {
              fakePlayer.QueueCardByCardID(CardBase.CardID.LEAH_BEAM);
            }
            fakePlayer.QueueCardByCardID(CardBase.CardID.LEAH_SHIELD);

            if (strikeCount >= 1)
            {
              fakePlayer.QueueCardByCardID(CardBase.CardID.LEAH_STRIKE);
              --strikeCount;
            }
            else
            {
              fakePlayer.QueueCardByCardID(CardBase.CardID.LEAH_BEAM);
            }

          }

          else
          {
            int sz = fakePlayer.m_deckMngr.m_hand.Count;
            for (int j = 0; j < sz && j < 3; ++j)
            {

              fakePlayer.QueueCardByCardID(fakePlayer.m_deckMngr.m_hand[0].Item1);

            }
          }
          foreach (CardBase.CardID j in currPerm.m_cardstoPlay)
          {

            fakeEnemy.QueueCardByCardID(j);
          }
          //Resolve the cards and combos
          for (int i = 0; i < 3; ++i)
          {
            CardBase playerCard = CardManager.Get(fakePlayer.m_deckMngr.m_queue[i].Item1);
            CardBase enemyCard = CardManager.Get(fakeEnemy.m_deckMngr.m_queue[i].Item1);
            if (playerCard.ID != CardBase.CardID.NO_CARD && playerCard.Type != CardType.SPECIAL)
            {
#if PRINT_PERM
              permMutex.WaitOne();
              Console.WriteLine("Player played: " + playerCard.ID);
              permMutex.ReleaseMutex();
#endif
              playerCard.Play(ref fakePlayer, ref fakeEnemy);
            }
            if (enemyCard.ID != CardBase.CardID.NO_CARD && enemyCard.Type != CardType.SPECIAL)
            {
#if PRINT_PERM
              permMutex.WaitOne();
              Console.WriteLine("Enemy played:  " + enemyCard.ID);
              permMutex.ReleaseMutex();
#endif
              enemyCard.Play(ref fakeEnemy, ref fakePlayer);
            }
            int pCalculatedDmg = fakePlayer.DamageDealt(i), eCalculatedDmg = fakeEnemy.DamageDealt(i);
#if PRINT_PERM
            permMutex.WaitOne();
            Console.WriteLine("Player deals: " + pCalculatedDmg);
            Console.WriteLine("Enemy deals:  " + eCalculatedDmg);
            permMutex.ReleaseMutex();
#endif
            fakePlayer.FakeTakeDamage(eCalculatedDmg);
            fakeEnemy.FakeTakeDamage(pCalculatedDmg);

            if (fakePlayer.IsDead() || fakeEnemy.IsDead())
            {
              break;
            }

            if (i > 0)
            {
              if (fakePlayer.m_deckMngr.m_queue[i - 1].Item1 != CardBase.CardID.NO_CARD && fakePlayer.m_deckMngr.m_queue[i].Item1 != CardBase.CardID.NO_CARD)
              {
#if PRINT_PERM
                permMutex.WaitOne();
                Console.Write("Player played combo: ");
                permMutex.ReleaseMutex();
#endif
                ComboManager.PlayCombo(ref fakePlayer, ref fakeEnemy, CardManager.Get(fakePlayer.m_deckMngr.m_queue[i - 1].Item1).Type, CardManager.Get(fakePlayer.m_deckMngr.m_queue[i].Item1).Type);
              }

              if (fakeEnemy.m_deckMngr.m_queue[i - 1].Item1 != CardBase.CardID.NO_CARD && fakeEnemy.m_deckMngr.m_queue[i].Item1 != CardBase.CardID.NO_CARD)
              {
#if PRINT_PERM
                permMutex.WaitOne();
                Console.Write("Enemy played combo:  ");
                permMutex.ReleaseMutex();
#endif
                ComboManager.PlayCombo(ref fakeEnemy, ref fakePlayer, CardManager.Get(fakeEnemy.m_deckMngr.m_queue[i - 1].Item1).Type, CardManager.Get(fakeEnemy.m_deckMngr.m_queue[i].Item1).Type);
              }
            }
            fakePlayer.ClearAtKBlk();
            fakeEnemy.ClearAtKBlk();
          }

          //Handle end of turn and draw cards.
          fakePlayer.FakeEndOfTurn();
          fakeEnemy.FakeEndOfTurn();
          fakePlayer.m_deckMngr.Draw();
          fakeEnemy.m_deckMngr.Draw();
#if PRINT_PERM
          permMutex.WaitOne();
          Console.WriteLine("After--------------------------------------------------");
          Console.WriteLine("Player Health:  " + fakePlayer.m_healthBar.m_health);
          Console.WriteLine("Player Hand:    " + fakePlayer.m_deckMngr.m_hand.Count);
          Console.WriteLine("Player Discard: " + fakePlayer.m_deckMngr.m_discard.Count);
          Console.WriteLine("Player Buff:    " + fakePlayer.m_buffs.Buffs.Count);
          Console.WriteLine("Enemy Health:   " + fakeEnemy.m_healthBar.m_health);
          Console.WriteLine("Enemy hand:     " + fakeEnemy.m_deckMngr.m_hand.Count);
          Console.WriteLine("Enemy discard:  " + fakeEnemy.m_deckMngr.m_discard.Count);
          Console.WriteLine("Enemy buff:     " + fakeEnemy.m_buffs.Buffs.Count);
          Console.WriteLine("--------------------------------------------------");
          Console.WriteLine("+++++++++++++++++++++++++++++++++++++++");
          permMutex.ReleaseMutex();
#endif
          ////Update the permutation
          currPerm.m_turnCount++;
          currPerm.m_playerHealth = fakePlayer.m_healthBar.m_health;
          currPerm.m_enemyHealth = fakeEnemy.m_healthBar.m_health;

          currPerm.m_cardstoPlay.Clear();
          for (int i = 0; i < fakeEnemy.m_deckMngr.m_hand.Count && i < 3; ++i)
          {
            currPerm.m_cardstoPlay.Add(fakeEnemy.m_deckMngr.m_hand[i].Item1);
          }

          //Console.WriteLine("Finished add card to enemy queue");

          currPerm.m_playerDeck = fakePlayer.m_deckMngr.m_deck;
          currPerm.m_enemyDeck = fakeEnemy.m_deckMngr.m_deck;
          currPerm.m_playerHand = fakePlayer.m_deckMngr.m_hand;
          currPerm.m_enemyHand = fakeEnemy.m_deckMngr.m_hand;
          currPerm.m_playerDiscard = fakePlayer.m_deckMngr.m_discard;
          currPerm.m_enemyDiscard = fakeEnemy.m_deckMngr.m_discard;
          currPerm.m_playerBuff = fakePlayer.m_buffs.Buffs;
          currPerm.m_enemyBuff = fakeEnemy.m_buffs.Buffs;
          //Console.WriteLine("thats one 1 turn");

          if (currPerm.m_turnCount == (m_maxTurn))
          {
            //#if PRINT_PERM
            permMutex.WaitOne();
            permutationList[currPermNum] = currPerm;
            permMutex.ReleaseMutex();
            //#endif
            
          }
        }
        currPermNum++;
      }

      permMutex.WaitOne();
      m_threadDone++;
      permMutex.ReleaseMutex();

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
      
      List<Task> tasks = new List<Task>();
      if (!m_simStarted)
      {
#if PRINT_PERM
        permMutex.WaitOne();
        Console.WriteLine("LETS START SIM");
        permMutex.ReleaseMutex();
#endif
        //m_currentSeq = 0;
        Stats playerS = (Stats)Utils.GetScript("Player", "Stats");
        Stats enemyS = (Stats)GetScriptFromID(entityID, "Stats");
#if PRINT_PERM
          permMutex.WaitOne();
          Console.WriteLine("Enemy(Before)************************************");
          Console.WriteLine("Enemy Health:   " + enemyS.m_healthBar.m_health);
          Console.WriteLine("Enemy hand:     " + enemyS.m_deckMngr.m_hand.Count);
          Console.WriteLine("Enemy discard:  " + enemyS.m_deckMngr.m_discard.Count);
          Console.WriteLine("Enemy buff:     " + enemyS.m_buffs.Buffs.Count);
          Console.WriteLine("--------------------------------------------------");
          Console.WriteLine("*******************************************");
          permMutex.ReleaseMutex();
#endif

        Permutation newP = new Permutation();
        //m_ranAllSeq = false;
        newP.m_turnCount = 0;
        newP.m_playerHealth = playerS.m_healthBar.m_health;
        newP.m_enemyHealth = enemyS.m_healthBar.m_health;
        newP.m_playerDeck = new Deck(playerS.m_deckMngr.m_deck);
        newP.m_enemyDeck = new Deck(enemyS.m_deckMngr.m_deck);
        newP.m_playerHand = new List<ValueTuple<CardBase.CardID, uint>>(playerS.m_deckMngr.m_hand); 
        newP.m_enemyHand = new List<ValueTuple<CardBase.CardID, uint>>(enemyS.m_deckMngr.m_hand);
        newP.m_playerDiscard = new List<CardBase.CardID>(playerS.m_deckMngr.m_discard);
        newP.m_enemyDiscard = new List<CardBase.CardID>(enemyS.m_deckMngr.m_discard);
        newP.m_playerBuff = new List<Buff>(playerS.m_buffs.Buffs);
        newP.m_enemyBuff = new List<Buff>(enemyS.m_buffs.Buffs);


        List<CardBase.CardID> choices = new List<CardBase.CardID>();
        for (int i = 0; i < enemyS.m_deckMngr.m_hand.Count(); ++i)
          choices.Add(enemyS.m_deckMngr.m_hand[i].Item1);
#if PRINT_PERM
        permMutex.WaitOne();
        Console.Write("CARDS IN HAND: ");
        foreach (CardBase.CardID newChoiceA in choices)
        {
          Console.Write(newChoiceA.ToString() + " ");
        }
        Console.WriteLine("\n");
        permMutex.ReleaseMutex();
#endif
        if (enemyS.m_deckMngr.m_hand.Count() >= 3)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            if (permutationList.Count >= m_maxSeqNum)
              break;
            CardBase.CardID choiceA = choices[i];
            List<CardBase.CardID> remainderA = new List<CardBase.CardID>(choices);
            remainderA.RemoveAt(i);
            for (int j = 0; j < remainderA.Count; ++j)
            {
              if (permutationList.Count >= m_maxSeqNum)
                break;
              CardBase.CardID choiceB = remainderA[j];
              List<CardBase.CardID> remainderB = new List<CardBase.CardID>(remainderA);
              remainderB.RemoveAt(j);
              for (int k = 0; k < remainderB.Count; ++k)
              {
                if (permutationList.Count >= m_maxSeqNum)
                  break;
                CardBase.CardID choiceC = remainderB[k];
                List<CardBase.CardID> newChoice = new List<CardBase.CardID> { choiceA, choiceB, choiceC };
#if PRINT_PERM
                permMutex.WaitOne();
                Console.Write("PLAYED: ");
                foreach (CardBase.CardID newChoiceA in newChoice)
                {
                  Console.Write(newChoiceA.ToString() + " ");
                }
                Console.WriteLine();
                permMutex.ReleaseMutex();
#endif
                newP.m_cardPlayed = new List<CardBase.CardID>(newChoice);
                newP.m_cardstoPlay = new List<CardBase.CardID>(newChoice);
                permutationList.Add(new Permutation(newP));
              }
            }
          }
        }

        if (enemyS.m_deckMngr.m_hand.Count() >= 2)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            if (permutationList.Count >= m_maxSeqNum)
              break;
            CardBase.CardID choiceA = choices[i];
            List<CardBase.CardID> remainderA = new List<CardBase.CardID>(choices);
            remainderA.RemoveAt(i);
            for (int j = 0; j < remainderA.Count; ++j)
            {
              if (permutationList.Count >= m_maxSeqNum)
                break;
              CardBase.CardID choiceB = remainderA[j];
              List<CardBase.CardID> newChoice = new List<CardBase.CardID> { choiceA, choiceB };
              newP.m_cardPlayed = new List<CardBase.CardID>(newChoice);
              newP.m_cardstoPlay = new List<CardBase.CardID>(newChoice);
              permutationList.Add(new Permutation(newP));
            }
          }
        }

        if (enemyS.m_deckMngr.m_hand.Count() >= 1)
        {
          for (int i = 0; i < choices.Count; ++i)
          {
            if (permutationList.Count >= m_maxSeqNum)
              break;
            List<CardBase.CardID> newChoice = new List<CardBase.CardID> { choices[i] };
            newP.m_cardPlayed = new List<CardBase.CardID>(newChoice);
            newP.m_cardstoPlay = new List<CardBase.CardID>(newChoice);
            permutationList.Add(new Permutation(newP));
          }
        }
        int numOfPerm = permutationList.Count;
        int numOfPermPerThread = numOfPerm / m_maxThread;
        Console.WriteLine("Num of Perm: " +numOfPerm);
        Console.WriteLine("Num of Perm per thread: " + numOfPermPerThread);

        for (int x=0;x< m_maxThread;++x)
        {
          int size = (x == m_maxThread - 1) ? numOfPermPerThread + (numOfPerm % m_maxThread) : numOfPermPerThread;
          int start = x * numOfPermPerThread;

          tasks.Add(Task.Run(() => SimulatePermutations(start, size)));
        }
        m_simStarted = true;
      }
      else
      {
          if(m_threadDone == m_maxThread)
          {

          Permutation bestPerm = permutationList[0];
          Stats enemyS = (Stats)GetScriptFromID(entityID, "Stats");
#if PRINT_PERM
          Console.WriteLine("0:: Player Health:" + bestPerm.m_playerHealth + "  Enemy Health:" + bestPerm.m_enemyHealth + " Enemy Cards in Hand: " + bestPerm.m_enemyHand.Count);
#endif
          int cc = 0;
          for (int i = 1;i<permutationList.Count();++i)
          {
#if PRINT_PERM
            Console.WriteLine(i + ":: Player Health:" + permutationList[i].m_playerHealth + "  Enemy Health:" + permutationList[i].m_enemyHealth + " Enemy Cards in Hand: " + permutationList[i].m_enemyHand.Count);
#endif
            List<int> pointsA = new List<int> { 0, 0, 0 };
            List<int> pointsB = new List<int>{ 0, 0, 0 };
            if(bestPerm.m_playerHealth != permutationList[i].m_playerHealth)
            {
              bestPerm = (bestPerm.m_playerHealth < permutationList[i].m_playerHealth) ? bestPerm : permutationList[i];
              cc = (bestPerm.m_playerHealth < permutationList[i].m_playerHealth) ? cc : i;
            }
            else if (bestPerm.m_enemyHealth != permutationList[i].m_enemyHealth)
            {
              bestPerm = (bestPerm.m_enemyHealth > permutationList[i].m_enemyHealth) ? bestPerm : permutationList[i];
              cc = (bestPerm.m_enemyHealth > permutationList[i].m_enemyHealth) ? cc : i;
            }
            else if(bestPerm.m_enemyHand.Count != permutationList[i].m_enemyHand.Count)
            {
              bestPerm = (bestPerm.m_enemyHand.Count > permutationList[i].m_enemyHand.Count) ? bestPerm : permutationList[i];
              cc = (bestPerm.m_enemyHand.Count > permutationList[i].m_enemyHand.Count) ? cc : i;
            }
              
          
          }
#if PRINT_PERM
          Console.WriteLine("Enemy(After)************************************");
          Console.WriteLine("Enemy Health:   " + enemyS.m_healthBar.m_health);
          Console.WriteLine("Enemy hand:     " + enemyS.m_deckMngr.m_hand.Count);
          Console.WriteLine("Enemy discard:  " + enemyS.m_deckMngr.m_discard.Count);
          Console.WriteLine("Enemy buff:     " + enemyS.m_buffs.Buffs.Count);
          Console.WriteLine("--------------------------------------------------");
          Console.WriteLine("*******************************************");
#endif
#if PRINT_PERM
          Console.WriteLine("Chose Perm: " + cc);
#endif
          foreach (CardID c in bestPerm.m_cardPlayed)
          {
            int ind = enemyS.m_deckMngr.m_hand.FindIndex(pair => pair.Item1 == c);
#if PRINT_PERM
            Console.WriteLine(ind);
#endif
            enemyS.QueueCard(ind);
          }
          permutationList.Clear();
          //m_currentSeq = 0;
          m_simStarted = false;
          m_threadDone = 0;
          //m_ranAllSeq = false;
          EndAI(entityID);
          }
      }
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
