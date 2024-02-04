﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;
using System.Threading;

namespace GoopScripts.Gameplay
{
  public class GameManager : Entity
  {
    
    static readonly uint PAUSE_MENU = 27;
    static readonly uint HOWTOPLAY_MENU= 56;
    static readonly uint QUIT_MENU = 60;


    Random m_rng;
    //double m_animTime = 1.0; // hard coded for now
    double m_currTime = 0.0;
    double m_animTime = 0.0;

    Stats m_playerStats, m_enemyStats;

    bool isResolutionPhase = false;  // flag for triggering a turn
    //bool intervalBeforeReset;

    //tools for resolving cards
    //List<CardBase.CardID> resolveQueue;
    int playerCardPos = 0;
    int enemyCardPos = 0;
    bool resolvePlayer = true;
    bool toTrigger = false;
    bool isStartOfTurn = true;


GameManager(uint entityID):base(entityID)
    {
      m_rng = new Random();
    }

    public void OnCreate()
    {
      Console.WriteLine("Create GameManager");
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
    }

    // function to allow c++ to edit the list of cards in cardmanager
    // this should use cardmanager's c++ interface function
    //  public void OnUpdate(double dt, Stats player, uint playerEntity, Stats enemy, uint enemyEntity, uint playerHand, uint playerQueue, uint enemyQueue)
    //{


    //    if (endTurn)
    //	{
    //      endTurn = false;
    //      //m_numResolves = 1;
    //		//m_numResolves = player.m_cardQueue.Length >= enemy.m_cardQueue.Length ? player.m_cardQueue.Length : enemy.m_cardQueue.Length;

    //		//// Do 1 turn of stuff
    //		//m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
    //      //m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

    //      //Utils.PlaySound(m_rng.Next(1, 1), playerEntity);
    //      //Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
    //      //Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
    //      //Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
    //      ////play anim?
    //      //Utils.PlayAnimation("SS_LeahShoot", playerEntity);

    //      //// remove card
    //      //player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
    //      //enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
    //      //++m_currResolves;
    //    }

    //    bool shouldEnd = true;
    //    if (m_currTime > m_animTime)
    //    {
    //      // Overflow the time
    //      m_currTime = m_animTime - m_currTime;

    //		// Do 1 turn of stuff
    //		//m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
    //		//m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

    //      //play anim? play sound?
    //      Utils.PlaySound(m_rng.Next(0, 1), playerEntity);
    //      //Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
    //      Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
    //      //Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);

    //      // remove card
    //      //player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
    //      //enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;

    //      //++m_currResolves;

    //		//if (m_currResolves >= m_numResolves)
    //		//{
    //		//	shouldEnd = true;
    //		//}
    //		Utils.PlayAnimation("SS_LeahShoot", playerEntity);
    //	}
    //    else
    //    {
    //		m_currTime += dt;
    //	}

    //    if (shouldEnd)
    //	{
    //		// Draw cards here.        
    //		Utils.GameSystemResolved();
    //      //int i = 0;
    //      //for (; i < player.m_hand.Length; ++i)
    //      //{
    //      //  if (player.m_hand[i] == CardBase.CardID.NO_CARD)
    //      //  {
    //      //    break;
    //      //  }
    //      //}
    //      //// Draw card if there is space, if no space burn card
    //      //if (i < player.m_hand.Length && player.m_deck.Length > 0)
    //      //{
    //      //  int drawnCard = m_rng.Next(0, player.m_deck.Length - 1);

    //      //  List<CardBase.CardID> temp = player.m_deck.ToList();
    //      //  temp.Remove((CardBase.CardID)drawnCard);
    //      //  player.m_deck = temp.ToArray();

    //      //  Utils.SetHandCardID(playerHand, i, (int)player.m_deck[drawnCard]);
    //      //  player.m_hand[i] = player.m_deck[drawnCard];
    //      //}
    //      //m_currResolves = 0;
    //      m_currTime = 0;

    //		Utils.PlayAnimation("SS_LeahIdle", playerEntity);
    //	}
    //  }

    public void OnUpdate(double deltaTime)
    {
      if (Utils.GetLoseFocus())
      {
        if (UI.PauseManager.GetPauseState() == 0)
          Utils.PauseMenu(PAUSE_MENU);
        Utils.SetLoseFocus(false);
      }
      if (Utils.IsKeyTriggered(Input.KeyCode.ESCAPE))
      {
        switch (UI.PauseManager.GetPauseState())
        {
        case 0:
            Utils.PauseMenu(PAUSE_MENU);
          break;
        case 1:
            Utils.UnpauseMenu(PAUSE_MENU);
          break;
        case 2:
            if (Utils.GetIsActiveEntity(HOWTOPLAY_MENU))
              Utils.UndeeperPause(PAUSE_MENU, HOWTOPLAY_MENU);
            if (Utils.GetIsActiveEntity(QUIT_MENU))
              Utils.UndeeperPause(PAUSE_MENU, QUIT_MENU);
            break;
        default:
          break;
        }
      }

      if (UI.PauseManager.PauseStateChanged())
      {
        // Console.WriteLine("Pause State has changed to: " + UI.PauseManager.GetPauseState());
      }

       if (isResolutionPhase)
       {
        //Console.WriteLine("IRPPPPPPPPPPPPP");
        ResolutionPhase(deltaTime);

        //var test = (UI.HealthBar)Utils.GetScriptFromID((uint)28, "HealthBar");
        //if (!testBool)
        //{
        //  testBool = true;
        //  test.DecreaseHealth(5);
        //}
        //else
        //{
        //  test.DecreaseHealth(1);

        //  //test.IncreaseHealth(1);
        //}
       // m_playerStats.TakeDamage(m_enemyStats.DamageDealt());
        //m_enemyStats.TakeDamage(m_playerStats.DamageDealt());
      }
     // m_playerStats.m_isSkipped = false;

      else
      {
       // Console.WriteLine("ELSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
        if (isStartOfTurn)
        {
          Console.WriteLine("START OF TURRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRn");
          isStartOfTurn = false;
          StartOfTurn();
          //intervalBeforeReset = false;
        }
         //m_currTime = 0.0;


      }
    }

    public void StartOfTurn()
    {
      m_playerStats.EndOfTurn();
      //Console.WriteLine("PLAYER END OF STAR F TURNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNnnn");
      m_enemyStats.EndOfTurn();
      //Console.WriteLine("ENEMY END OF STAR F TURNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNnnn");
      m_playerStats.m_deckMngr.Draw();
     // Console.WriteLine("ENEMY DRAW  TURNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNnnn");
      StartAI(m_enemyStats.entityID);
      //Console.WriteLine("END OF STAR F TURNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNnnn");
    }



      public void ResolutionPhase(double deltaTime)
    {
#if (DEBUG)
      //Console.WriteLine("Player Queue:");

      //foreach (CardBase.CardID c in m_playerStats.m_deckMngr.m_queue)
      //{
      //  Console.WriteLine(c.ToString());
      //}
#endif
      Console.WriteLine("currTime:: " + m_currTime);
      if(toTrigger)
      {
        //Console.WriteLine("time to triggER");
        toTrigger = false;
        if(resolvePlayer)
        {
          
          CardBase.CardID card = m_playerStats.m_deckMngr.m_queue[playerCardPos];
          //Console.WriteLine("RESO PLAYER"+ card);
          if(card != CardBase.CardID.NO_CARD)
          {
            CardManager.Get(card).Play(ref m_playerStats, ref m_enemyStats);
            m_animTime = Utils.GetAnimationTime(CardManager.Get(card).SpriteAnimation);
            Console.WriteLine("ANIMATION TIME:: " + m_animTime);
            Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_playerStats.entityID);
          }
          else
            ResolveNextCard();

        }
        else
        {
          ComboManager.ComboPlayer(ref m_playerStats, ref m_enemyStats);
          CardBase.CardID card = m_enemyStats.m_deckMngr.m_queue[enemyCardPos];
          //Console.WriteLine("RESO ENEMY" + card);
          if (card != CardBase.CardID.NO_CARD)
          {
            CardManager.Get(card).Play(ref m_enemyStats, ref m_playerStats);
            m_animTime = Utils.GetAnimationTime(CardManager.Get(card).SpriteAnimation);
            Console.WriteLine("ANIMATION TIME:: " + m_animTime);
            Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_enemyStats.entityID);
          }
          else
            ResolveNextCard();
        }
      }

      m_currTime += deltaTime;
      if(m_currTime>= m_animTime)
      {
        ResolveNextCard();
      }
      // resolve player's queue first
      //foreach (CardBase.CardID card in m_playerStats.m_deckMngr.m_queue)
      //{
      //  if (card == CardBase.CardID.NO_CARD) { continue; }

#if (DEBUG)
      //Console.WriteLine("Resolving " + card.ToString());
#endif
        //CardManager.Get(card).Play(ref m_playerStats, ref m_enemyStats);
        //Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_playerStats.entityID);

        //double AnimationInterval = 2.0 ;
        //double currTime = 0.0;
        //bool playAnimation = true;
        //while (playAnimation)
        //{
        //  currTime += 0.001;
        //  Console.WriteLine("TEST\n");
        //  if (currTime >= AnimationInterval)
        //  {
        //    currTime = 0;
        //    playAnimation = false;
        //  }
        
        //}


        // Create a Timer with a callback function

        
      //  Console.WriteLine("Sprite: " + CardManager.Get(card).SpriteAnimation + ":: " + m_playerStats.entityID);
      //}
      //ComboManager.ComboPlayer(ref m_playerStats, ref m_enemyStats);

      //// then do the same for enemy
      //foreach (CardBase.CardID card in m_enemyStats.m_deckMngr.m_queue)
      //{
      //  if (card != CardBase.CardID.NO_CARD)
      //  {
      //    Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_enemyStats.entityID);
      //    CardManager.Get(card).Play(ref m_enemyStats, ref m_playerStats);
      //    double AnimationInterval = 2.0;
      //    double currTime = 0.0;
      //    bool playAnimation = true;
      //    while (playAnimation)
      //    {
      //      currTime += deltaTime;

      //      if (currTime >= AnimationInterval)
      //      {
      //        currTime = 0.0;
      //        playAnimation = false;
      //      }
          
      //    }


      //    Console.WriteLine("Sprite: " + CardManager.Get(card).SpriteAnimation + ":: " + m_enemyStats.entityID);
      //  }
      //}
      //ComboManager.ComboEnemy(ref m_playerStats, ref m_enemyStats);

#if (DEBUG)
      //Console.WriteLine("\nPLAYER:\n Attack: " + m_playerStats.m_attack + ", Block: " + m_playerStats.m_block);
      //Console.WriteLine("\nENEMY:\n Attack: " + m_enemyStats.m_attack + ", Block: " + m_enemyStats.m_block + "\n");
#endif
    }

    private void StartResolution()
    {
      playerCardPos = 0;
      enemyCardPos = 0;
      Console.WriteLine("START RESO");
      resolvePlayer = true;
      toTrigger = true;
    }

    private void ResolveNextCard()
    {
      m_currTime = 0;
      if(resolvePlayer)
      {
        Console.WriteLine("FINSIHED PLAYER ANIMATION");
        playerCardPos += 1;
        while(playerCardPos < m_enemyStats.m_deckMngr.m_queue.Length && m_playerStats.m_deckMngr.m_queue[playerCardPos] == CardBase.CardID.NO_CARD )
        {
          playerCardPos++;
        }

        if (enemyCardPos <= m_enemyStats.m_deckMngr.m_queue.Length)
        {
          Console.WriteLine("RESO ENEMY NEXT");
          toTrigger = true;
          resolvePlayer = !resolvePlayer;
        }

        else
        {
          if (playerCardPos >= m_playerStats.m_deckMngr.m_queue.Length)
            isResolutionPhase = false; 
           else
             toTrigger = true;
          

        }
      }
      else
      {
        Console.WriteLine("FINSIHED ENEMT ANIMATION");
        enemyCardPos += 1;
        while (enemyCardPos < m_enemyStats.m_deckMngr.m_queue.Length && m_enemyStats.m_deckMngr.m_queue[enemyCardPos] == CardBase.CardID.NO_CARD)
        {
          enemyCardPos ++;
        }
        if (playerCardPos < m_playerStats.m_deckMngr.m_queue.Length)
        {
          Console.WriteLine("RESO PLAYER NEXT");
          toTrigger = true;
          resolvePlayer = !resolvePlayer;
        }
        else
        {
          if (enemyCardPos >= m_enemyStats.m_deckMngr.m_queue.Length)
             isResolutionPhase = false;
           else
              toTrigger = true;
          
           
        }
      }
    }

    public void EndTurn()
    {
      //if (intervalBeforeReset) { return; }
      Console.WriteLine("END TURN");
      isResolutionPhase = true;
      StartResolution();
    }
  }
}
