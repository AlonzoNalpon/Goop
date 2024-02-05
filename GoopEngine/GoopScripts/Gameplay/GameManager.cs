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
using System.Threading;

namespace GoopScripts.Gameplay
{
  public class GameManager : Entity
  {
    //static readonly double INTERVAL_TIME = 3.0;
    public int PAUSE_MENU;
    public int HOWTOPLAY_MENU;
    public int QUIT_MENU;

    Random m_rng;
    double m_currTime = 0.0;
    double m_animTime = 0.0;


    public Stats m_playerStats, m_enemyStats;

    bool isResolutionPhase = false;  // flag for triggering a turn
    //bool intervalBeforeReset;

    //tools for resolving cards
    //List<CardBase.CardID> resolveQueue;
    int playerCardPos = 0;
    int enemyCardPos = 0;
    bool resolvePlayer = true;
    bool toTrigger = false;
    bool isStartOfTurn = true;
    List<CardBase.CardID> m_cardsPlayedP = new List<CardBase.CardID>();
    List<CardBase.CardID> m_cardsPlayedE = new List<CardBase.CardID>();
    bool gameStarted = false; // called once at the start of game 

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

    public void OnUpdate(double deltaTime)
    {
      if (!gameStarted)
      {
        m_playerStats.Init();
        m_enemyStats.Init();
        gameStarted = true;
        //m_playerStats.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));
        //m_playerStats.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 0, "Skipped"));
        //m_enemyStats.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 0, "Blinded"));
        //m_enemyStats.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 0, "Block Up"));
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
          isStartOfTurn = false;
          StartOfTurn();
          //intervalBeforeReset = false;
        }
         //m_currTime = 0.0;


      }
    }

    public void StartOfTurn()
    {
      m_playerStats.m_deckMngr.DiscardQueue();
      m_enemyStats.m_deckMngr.DiscardQueue();
      m_playerStats.m_deckMngr.Draw();
      StartAI(m_enemyStats.entityID);

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
      // Console.WriteLine("currTime:: " + m_currTime);
      if (toTrigger)
      {
        //Console.WriteLine("time to triggER");
        toTrigger = false;
        if (resolvePlayer)
        {

          CardBase.CardID card = m_playerStats.m_deckMngr.m_queue[playerCardPos].Item1;
          if (card != CardBase.CardID.NO_CARD)
          {
            CardManager.Get(card).Play(ref m_playerStats, ref m_enemyStats);
            m_animTime = Utils.GetAnimationTime(CardManager.Get(card).SpriteAnimation);
            m_cardsPlayedP.Add(card);
            Console.WriteLine("PLAYER ANIM ERROR: " + m_playerStats.entityID);
            Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_playerStats.entityID);
          }
          else
            ResolveNextCard();

        }
        else
        {

          CardBase.CardID card = m_enemyStats.m_deckMngr.m_queue[enemyCardPos].Item1;
          if (card != CardBase.CardID.NO_CARD)
          {
            CardManager.Get(card).Play(ref m_enemyStats, ref m_playerStats);
            m_animTime = Utils.GetAnimationTime(CardManager.Get(card).SpriteAnimation);
            m_cardsPlayedE.Add(card);
            Console.WriteLine("ENEMY ANIM ERROR: " + m_playerStats.entityID);
            Utils.PlayAnimation(CardManager.Get(card).SpriteAnimation, m_enemyStats.entityID);
          }
          else
            ResolveNextCard();
        }
      }
      else
      {
        m_currTime += deltaTime;
        if (m_currTime >= m_animTime)
        {
          Console.WriteLine("PLayer and ENEMY ANIM ERROR: " + m_playerStats.entityID +"," + m_enemyStats.entityID);
          Utils.PlayAnimation("SS_Leah_Idle", m_playerStats.entityID);
          Utils.PlayAnimation("SS_MoleRat_Idle", m_enemyStats.entityID);
          if (resolvePlayer)
          {
            if (m_cardsPlayedP.Count >= 2)
            {
              int firstCardPos = (m_cardsPlayedP.Count < 3) ? 0 : 1;
              ComboManager.Combo(ref m_playerStats, ref m_enemyStats, firstCardPos);
            }
          }

          else
          {
            if (m_cardsPlayedE.Count >= 2)
            {
              int firstCardPos = (m_cardsPlayedE.Count < 3) ? 0 : 1;
              ComboManager.Combo(ref m_enemyStats, ref m_playerStats, firstCardPos);
            }
          }
          ResolveNextCard();
        }
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
      m_cardsPlayedP.Clear();
      m_cardsPlayedE.Clear();
      resolvePlayer = true;
      toTrigger = true;

      //Console.WriteLine($"Number of cards in queue: {source.m_deckMngr.m_queue.Length}");
      //Console.WriteLine($"Card types in Player queue:");
      //foreach (var CardID in m_playerStats.m_deckMngr.m_queue)
      //{
      //  Console.WriteLine($"{CardManager.Get(CardID).Type.ToString()}");
      //}

      //Console.WriteLine($"Card types in Enemy queue:");
      //foreach (var CardID in m_enemyStats.m_deckMngr.m_queue)
      //{
      //  Console.WriteLine($"{CardManager.Get(CardID).Type.ToString()}");
      //}

      //Console.WriteLine("");
    }

    private void ResolveNextCard()
    {
      m_currTime = 0;
      if(resolvePlayer)
      {
        Console.WriteLine("FINSIHED PLAYER CARD");
        playerCardPos += 1;
        while(playerCardPos < m_enemyStats.m_deckMngr.m_queue.Length && m_playerStats.m_deckMngr.m_queue[playerCardPos].Item1 == CardBase.CardID.NO_CARD )
        {
          playerCardPos++;
        }

        if (enemyCardPos < m_enemyStats.m_deckMngr.m_queue.Length)
        {
          Console.WriteLine("RESO ENEMY NEXT");
          toTrigger = true;
          resolvePlayer = !resolvePlayer;
        }

        else
        {
          if (playerCardPos >= m_playerStats.m_deckMngr.m_queue.Length)
          {
            isResolutionPhase = false;
            isStartOfTurn = true;
          }
           
           else
             toTrigger = true;
          

        }
      }
      else
      {
        Console.WriteLine("FINSIHED ENEMY CARD");
        enemyCardPos += 1;
        while (enemyCardPos < m_enemyStats.m_deckMngr.m_queue.Length && m_enemyStats.m_deckMngr.m_queue[enemyCardPos].Item1 == CardBase.CardID.NO_CARD)
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
          {
            isResolutionPhase = false;
            isStartOfTurn = true;
          }
            
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
