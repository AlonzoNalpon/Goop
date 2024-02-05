using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Gameplay
{
  public class GameManager : Entity
  {
    //static readonly double INTERVAL_TIME = 3.0;
    public int PAUSE_MENU;
    public int HOWTOPLAY_MENU;
    public int QUIT_MENU;

    Random m_rng;
    //double m_animTime = 1.0; // hard coded for now
    //double m_currTime = 0.0;

    public Stats m_playerStats, m_enemyStats;

    bool gameStarted = false; // called once at the start of game 
    bool endTurn = false;  // flag for triggering a turn
    bool intervalBeforeReset;

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

    //public void OnUpdate(double deltaTime)
    //{
    //  // handle input
    //  if (Utils.IsKeyTriggered(Input.KeyCode.C))
    //  {
    //    if (UI.PauseManager.IsPaused())
    //    {
    //      if (UI.PauseManager.IsDeeperPaused())
    //        UndeeperPause();
    //      else
    //        UnpauseMenu();
    //    }
    //    else
    //      PauseMenu();
    //  }

    //  if (Utils.IsKeyTriggered(Input.KeyCode.X))
    //  {
    //    if (UI.PauseManager.IsPaused())
    //      DeeperPauseMenu();
    //  }


    //  // game logic
    //  if (endTurn)
    //  {
    //    endTurn = false;
    //    intervalBeforeReset = true;
    //    ResolutionPhase();
    //  }

    //  if (intervalBeforeReset)
    //  {
    //    m_currTime += deltaTime;

    //    if (m_currTime >= INTERVAL_TIME)
    //    {
    //      m_currTime = 0.0;
    //      m_playerStats.TakeDamage(m_enemyStats.DamageDealt());
    //      m_enemyStats.TakeDamage(m_playerStats.DamageDealt());
    //      StartOfTurn();
    //      intervalBeforeReset = false;
    //    }
    //  }
    //}

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
      if (!gameStarted)
      {
        m_playerStats.Init();
        m_enemyStats.Init();
        gameStarted = true;
      }

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
            if (Utils.GetIsActiveEntity((uint)HOWTOPLAY_MENU))
              Utils.UndeeperPause(PAUSE_MENU, HOWTOPLAY_MENU);
            if (Utils.GetIsActiveEntity((uint)QUIT_MENU))
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

      //endTurn = true;
      if (endTurn || m_playerStats.m_isSkipped)
      {
        endTurn = false;
        intervalBeforeReset = true;
        ResolutionPhase();
      }
    }

    public void StartOfTurn()
    {
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      m_playerStats.m_deckMngr.Draw();
      m_enemyStats.m_deckMngr.Draw();

      if (!m_enemyStats.m_isSkipped)
        StartAI(m_enemyStats.entityID);
    }

    public void ResolutionPhase()
    {
#if (DEBUG)
      Console.WriteLine("Player Queue:");

      foreach (var c in m_playerStats.m_deckMngr.m_queue)
      {
        Console.WriteLine(c.Item1.ToString());
      }
#endif

      // resolve player's queue first
      foreach (var card in m_playerStats.m_deckMngr.m_queue)
      {
        if (card.Item1 == CardBase.CardID.NO_CARD)
        {
          continue;
        }

#if (DEBUG)
        Console.WriteLine("Resolving " + card.ToString());
#endif

        CardManager.Get(card.Item1).Play(ref m_playerStats, ref m_enemyStats);
      }
      ComboManager.Combo(ref m_playerStats, ref m_enemyStats);

      // then do the same for enemy
      foreach (var card in m_enemyStats.m_deckMngr.m_queue)
      {
        if (card.Item1 != CardBase.CardID.NO_CARD)
        {
#if (DEBUG)
          Console.WriteLine("[Enemy] Resolving " + card.ToString());
#endif

          CardManager.Get(card.Item1).Play(ref m_enemyStats, ref m_playerStats);
        }
      }
      ComboManager.Combo(ref m_enemyStats, ref m_playerStats);

#if (DEBUG)
      Console.WriteLine("\nPLAYER:\n Attack: " + m_playerStats.m_attack + ", Block: " + m_playerStats.m_block);
      Console.WriteLine("\nENEMY:\n Attack: " + m_enemyStats.m_attack + ", Block: " + m_enemyStats.m_block + "\n");
#endif
      StartOfTurn();
    }

    public void EndTurn()
    {
      if (intervalBeforeReset) { return; }
      endTurn = true;
    }
  }
}
