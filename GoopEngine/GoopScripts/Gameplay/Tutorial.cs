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
using static GoopScripts.Cards.CardBase;

namespace GoopScripts.Gameplay
{
  public class Tutorial : Entity
  {
    public int PAUSE_MENU;
    public int HOWTOPLAY_MENU;
    public int QUIT_MENU;
    public int FPS_COUNTER;

    int m_turn = 1;

    Random m_rng;
    double m_currTime = 0.0;
    double m_animTime = 0.0;


    public Stats m_playerStats, m_enemyStats;

    static bool isResolutionPhase = false;  // flag for triggering a turn
    
    int m_slotNum = 0;
    bool toTrigger = false;
    bool isStartOfTurn = true;
    List<CardBase.CardID> m_cardsPlayedP = new List<CardBase.CardID>();
    List<CardBase.CardID> m_cardsPlayedE = new List<CardBase.CardID>();
    bool gameStarted = false; // called once at the start of game 
    List<CardBase.CardID> m_playerNonAtkCards = new List<CardBase.CardID>{ CardID.LEAH_SHIELD, CardID.SPECIAL_SMOKESCREEN, CardID.SPECIAL_RAGE };
    List<CardBase.CardID> m_enemyNonAtkCards = new List<CardBase.CardID> { CardID.DAWSON_SHIELD, CardID.BASIC_SHIELD};

    Tutorial(uint entityID):base(entityID)
    {
      m_rng = new Random();
    }

    public void OnCreate()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
      UI.PauseManager.SetPauseState(0);

      for (int i = 0; i < m_playerStats.m_deckMngr.m_deck.Size(); i++)
      {
        m_playerStats.m_deckMngr.m_deck.BurnTop();
      }
      for (int i = 0; i < m_enemyStats.m_deckMngr.m_deck.Size(); i++)
      {
        m_enemyStats.m_deckMngr.m_deck.BurnTop();
      }

      //player deck
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_SHIELD);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_BEAM);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_STRIKE);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.SPECIAL_FLASHBANG);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_SHIELD);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_STRIKE);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_SHIELD);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_BEAM);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_SHIELD);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_SHIELD);
      m_playerStats.m_deckMngr.m_deck.AddCard(CardID.LEAH_STRIKE);

      //enemy deck
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_ATTACK);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_SHIELD);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_ATTACK);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_ATTACK);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_ATTACK);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_ATTACK);
      m_enemyStats.m_deckMngr.m_deck.AddCard(CardID.BASIC_SHIELD);
    }

    public void OnUpdate(double deltaTime)
    {
      {
        // SET FPS TEXT TO FPS
        double fps = Utils.GetFPS();
        string text = string.Format("{0:N2}", fps);
        Utils.SetTextComponent(FPS_COUNTER, text);
      }

      if (!gameStarted)
      {
        m_playerStats.Init();
        m_enemyStats.Draw();
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

      if (isResolutionPhase)
      {
        ResolutionPhase(deltaTime);
      }


      else
      {
        if (isStartOfTurn)
        {
          isStartOfTurn = false;
          StartOfTurn();
        }
      }
    }

    public void StartOfTurn()
    {
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      ++m_turn;
      m_playerStats.Draw();
      if (m_turn == 4)
      {
        m_enemyStats.Draw();
        m_enemyStats.Draw();
      }
      else
      {
        m_enemyStats.Draw();
      }
      
      for (int i = 0; i < m_enemyStats.m_deckMngr.m_deck.Size(); i++)
      {
        m_enemyStats.QueueCard(i);
      }
    }



    public void ResolutionPhase(double deltaTime)
    {
      //Time to trigger the card effects and the animations.
      if (toTrigger)
      {
        toTrigger = false;
        bool toTriggerPlayerTakeDmg = true;
        bool toTriggerEnemyTakeDmg = true;
        m_animTime = 0.0f;
        CardBase.CardID playerCard = m_playerStats.m_deckMngr.m_queue[m_slotNum].Item1;
        CardBase.CardID enemyCard = m_enemyStats.m_deckMngr.m_queue[m_slotNum].Item1;
        m_cardsPlayedP.Add(playerCard);
        m_cardsPlayedE.Add(enemyCard);

        //Play and resolve player and enemy card if they r valid cards
        if (playerCard != CardBase.CardID.NO_CARD)
        {
          CardManager.Get(playerCard).Play(ref m_playerStats, ref m_enemyStats);
          double m_pAnimTime = Utils.GetAnimationTime(CardManager.Get(playerCard).SpriteAnimation);
          m_animTime = (m_animTime < m_pAnimTime) ? m_pAnimTime : m_animTime;
          Utils.PlayAnimation(CardManager.Get(playerCard).SpriteAnimation, m_playerStats.entityID);
          toTriggerPlayerTakeDmg = false;
        }

        if (enemyCard != CardBase.CardID.NO_CARD)
        {
          CardManager.Get(enemyCard).Play(ref m_enemyStats, ref m_playerStats);
          double m_pAnimTime = Utils.GetAnimationTime(CardManager.Get(enemyCard).SpriteAnimation);
          m_animTime = (m_animTime < m_pAnimTime) ? m_pAnimTime : m_animTime;
          Utils.PlayAnimation(CardManager.Get(enemyCard).SpriteAnimation, m_enemyStats.entityID);
          toTriggerEnemyTakeDmg = false;
        }

        //Both player and enemy did nothing, we just skip this slot
        if (playerCard == CardBase.CardID.NO_CARD && enemyCard == CardBase.CardID.NO_CARD)
        {
          m_animTime = 0.0;
        }

        //Either side or Both played a valid card
        else
        {
          //Check if player shld play idle animation or play take damage
          if(toTriggerPlayerTakeDmg)
          {
            if (m_enemyNonAtkCards.Contains(enemyCard))
              Utils.PlayAnimation("SS_Leah_Idle", m_playerStats.entityID);
            else
              Utils.PlayAnimation("SS_Leah_Flinch", m_playerStats.entityID);
          }

          //Check if enemy shld play idle animation or play take damage
          if (toTriggerEnemyTakeDmg)
          {
            if (m_playerNonAtkCards.Contains(playerCard))
              Utils.PlayAnimation("SS_MoleRat_Idle", m_enemyStats.entityID);
            else
              Utils.PlayAnimation("SS_MoleRat_Curl", m_enemyStats.entityID);
          }

          // this should not be coded here
          // move in future pls
          int pCalculatedDmg = m_playerStats.DamageDealt(), eCalculatedDmg = m_enemyStats.DamageDealt();

          //We have resolved more than 1 slot, lets see if the player or the enemy can perform combo
          if (m_slotNum > 0)
          {
            if (m_cardsPlayedP[m_slotNum - 1] != CardBase.CardID.NO_CARD && m_cardsPlayedP[m_slotNum] != CardBase.CardID.NO_CARD)
            {
              ComboManager.Combo(ref m_playerStats, ref m_enemyStats, (m_slotNum - 1));
            }


            if (m_cardsPlayedE[m_slotNum - 1] != CardBase.CardID.NO_CARD && m_cardsPlayedE[m_slotNum] != CardBase.CardID.NO_CARD)
            {
              ComboManager.Combo(ref m_enemyStats, ref m_playerStats, (m_slotNum - 1));
            }
          }

          m_playerStats.TakeDamage(eCalculatedDmg);
          m_enemyStats.TakeDamage(pCalculatedDmg);
          // bad code but for demo ok
          if (m_playerStats.IsDead())
          {
            Utils.PlayAnimation("SS_Leah_Death", m_playerStats.entityID);
          }
          else if (m_enemyStats.IsDead())
          {
            Utils.PlayAnimation("SS_MoleRat_Death", m_enemyStats.entityID);
          }

          m_playerStats.ClearAtKBlk();
          m_enemyStats.ClearAtKBlk();
        }

        ++m_slotNum;


      }
      else
      {

        if(m_currTime >=m_animTime)
        {
          m_currTime = 0.0;
          if (m_slotNum > 2) //we have resolved all the animation for this round
          {
            Utils.PlayAnimation("SS_Leah_Idle", m_playerStats.entityID);
            Utils.PlayAnimation("SS_MoleRat_Idle", m_enemyStats.entityID);
            isResolutionPhase = false;
            isStartOfTurn = true;

            // bad code but for demo ok
            if (m_playerStats.IsDead())
            {
              // defeat
              TransitionToScene("Defeat");
            }
            else if (m_enemyStats.IsDead())
            {
              // victory
              TransitionToScene("Victory");
            }
          }

          else //we have not resolve all the animaiton for this round, lets continue
             toTrigger = true;

        }
        


      }

      ++m_currTime;
    }

    private void StartResolution()
    {
      m_slotNum = 0;
      m_cardsPlayedP.Clear();
      m_cardsPlayedE.Clear();
      toTrigger = true;
      m_currTime = 0.0;
    }

  

    public void EndTurn()
    {
      isResolutionPhase = true;
      StartResolution();
    }
  }
}
