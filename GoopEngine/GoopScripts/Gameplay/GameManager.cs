/*!*********************************************************************
\file       GameManager.cs
\author     chengen.lau\@digipen.edu
\co-author  c.phua\@digipen.edu, han.q@digipen.edu
\date       10-January-2024
\brief      The main update loop for the game scene. A single entity
            "GameSystem" will run this script in the scene.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
using GoopScripts.Button;

namespace GoopScripts.Gameplay
{
  public class GameManager : Entity
  {
    //static readonly double INTERVAL_TIME = 3.0;
    public int PAUSE_MENU, HOWTOPLAY_MENU, QUIT_MENU;
    public int P_QUEUE_HIGHLIGHT, E_QUEUE_HIGHLIGHT;

    Random m_rng;
    double m_currTime = 0.0;
    double m_animTime = 0.0;


    public Stats m_playerStats, m_enemyStats;

    static bool isResolutionPhase = false;
    static bool isDead = false;
    //bool intervalBeforeReset;

    //tools for resolving cards
    //List<CardBase.CardID> resolveQueue;
    int m_slotNum = 0;
    bool toTrigger = false;
    bool isStartOfTurn = true;
    List<CardBase.CardID> m_cardsPlayedP = new List<CardBase.CardID>();
    List<CardBase.CardID> m_cardsPlayedE = new List<CardBase.CardID>();
    bool gameStarted = false; // called once at the start of game 
    List<CardBase.CardID> m_playerNonAtkCards = new List<CardBase.CardID>{ CardID.LEAH_SHIELD, CardID.SPECIAL_SMOKESCREEN, CardID.SPECIAL_RAGE };
    List<CardBase.CardID> m_enemyNonAtkCards = new List<CardBase.CardID> { CardID.DAWSON_SHIELD, CardID.BASIC_SHIELD };



    GameManager(uint entityID):base(entityID)
    {
      m_rng = new Random();
    }

    /*!*********************************************************************
      \brief
        OnCreate function for GameManager
      ************************************************************************/
    public void OnCreate()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
      UI.PauseManager.SetPauseState(0);

      // set the static variable to the entity holding the hover effect sprite
      SelectCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(SelectCard.m_cardHover, false);
      isDead = false;
    }


    /*!*********************************************************************
      \brief
        OnUpdate function for GameManager
      \param deltaTime
        delta time since last frame
      ************************************************************************/
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
      // cheat code to deal damage
      if (Utils.IsKeyTriggered(Input.KeyCode.U))
      {
        m_enemyStats.m_healthBar.DecreaseHealth(1);
      }

      if (UI.PauseManager.PauseStateChanged())
      {
        if (UI.PauseManager.GetPauseState() != 0)
        {
          Utils.SetTimeScale(0.0f);
        }
        else
        {
          Utils.SetTimeScale(1.0f);
        }
        // Console.WriteLine("Pause State has changed to: " + UI.PauseManager.GetPauseState());
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

    /*!*********************************************************************
      \brief
        This function is triggered when the start of turn occurs. Player and enemy will draw cards. Also triggers the Enemy's AI
      ************************************************************************/
    public void StartOfTurn()
    {
      SetHighlightActive(false);
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      m_playerStats.Draw();
      m_enemyStats.Draw();
      StartAI(m_enemyStats.entityID);      
    }



    /*!*********************************************************************
      \brief
        This function handles the resolution phase. It plays the animation based on the cards played.
        It also trigger combos if player/enemy plays more than 1 card
      \param deltaTime
        delta time since last frame
      ************************************************************************/
    public void ResolutionPhase(double deltaTime)
    {
      //Time to trigger the card effects and the animations.
      if (toTrigger && !isDead)
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
          //Console.WriteLine("Play " + playerCard.ToString());
          CardManager.Get(playerCard).Play(ref m_playerStats, ref m_enemyStats);
          double m_pAnimTime = Utils.GetAnimationTime(CardManager.Get(playerCard).SpriteAnimation);
          m_animTime = (m_animTime < m_pAnimTime) ? m_pAnimTime : m_animTime;
          Utils.PlayAnimation(CardManager.Get(playerCard).SpriteAnimation, m_playerStats.entityID);
          toTriggerPlayerTakeDmg = false;
        }

        if (enemyCard != CardBase.CardID.NO_CARD)
        {
          //Console.WriteLine("Play " + enemyCard.ToString());
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
              //Console.WriteLine("Player COMBOED");
              ComboManager.Combo(ref m_playerStats, ref m_enemyStats, (m_slotNum - 1));
            }


            if (m_cardsPlayedE[m_slotNum - 1] != CardBase.CardID.NO_CARD && m_cardsPlayedE[m_slotNum] != CardBase.CardID.NO_CARD)
            {
              //Console.WriteLine("ENEMY COMBOED");
              ComboManager.Combo(ref m_enemyStats, ref m_playerStats, (m_slotNum - 1));
            }
          }

          m_playerStats.TakeDamage(eCalculatedDmg);
          m_enemyStats.TakeDamage(pCalculatedDmg);
          double deathTime = 0.0;
          // bad code but for demo ok
          if (m_playerStats.IsDead())
          {
            Utils.PlayAnimation("SS_Leah_Death", m_playerStats.entityID);
            deathTime= (Utils.GetAnimationTime("SS_Leah_Death") > deathTime)? Utils.GetAnimationTime("SS_Leah_Death") : deathTime;
            isDead = true;

          }
          else if (m_enemyStats.IsDead())
          {
            Utils.PlayAnimation("SS_MoleRat_Death", m_enemyStats.entityID);
            deathTime = (Utils.GetAnimationTime("SS_MoleRat_Death") > deathTime) ? Utils.GetAnimationTime("SS_MoleRat_Death") : deathTime;
            isDead = true;
          }

          m_animTime = (deathTime>m_animTime)? deathTime:m_animTime;
          m_playerStats.ClearAtKBlk();
          m_enemyStats.ClearAtKBlk();
        }

        HighlightQueueSlot(m_slotNum);
        ++m_slotNum;
      }
      else
      {

        if(m_currTime >=m_animTime)
        {
          m_currTime = 0.0;
          if (isDead)
          {
            // bad code but for demo ok
            if (m_playerStats.IsDead())
            {
              // defeat
              Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Defeat");
            }
            else if (m_enemyStats.IsDead())
            {
							// victory
							Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Victory");
						}
          }
          else
          {
            if (m_slotNum > 2) //we have resolved all the animation for this round
            {
              Utils.PlayAnimation("SS_Leah_Idle", m_playerStats.entityID);
              Utils.PlayAnimation("SS_MoleRat_Idle", m_enemyStats.entityID);
              isResolutionPhase = false;
              isStartOfTurn = true;


            }

            else //we have not resolve all the animaiton for this round, lets continue
              toTrigger = true;
          }
        }
      }

      ++m_currTime;
    }


    /*!*********************************************************************
      \brief
        This function is triggered t the satrt of resolution phase. This reset the variables
        used when resolving the resolution phase
      ************************************************************************/
    private void StartResolution()
    {
      m_slotNum = 0;
      m_cardsPlayedP.Clear();
      m_cardsPlayedE.Clear();
      toTrigger = true;
      m_currTime = 0.0;
      SetHighlightActive(true);
    }


    /*!*********************************************************************
      \brief
        This funciton is triggered when the user clicks the end turn button. THis function
        starts the resolution phase
      ************************************************************************/
    public void EndTurn()
    {
      //Console.WriteLine("END TURN");
      isResolutionPhase = true;
      StartResolution();
    }

    /*!*********************************************************************
		\brief
		  Sets both highlight prefab instances to the given state.
    \param state
      The state to set the entity to
		************************************************************************/
    public void SetHighlightActive(bool state)
    {
      Utils.SetIsActiveEntity((uint)P_QUEUE_HIGHLIGHT, state);
      Utils.SetIsActiveEntity((uint)E_QUEUE_HIGHLIGHT, state);
    }

    /*!*********************************************************************
		\brief
		  Highlights an icon in both queues based on the given index.
      Sets the position of the highlight prefab instance to the slot's
      position.
    \param index
      The index of the queue to highlight
		************************************************************************/
    public void HighlightQueueSlot(int index)
    {
      Vec3<double> pPos = new Vec3<double>(m_playerStats.m_queueElemPos[index]);
      Vec3<double> ePos = new Vec3<double>(m_enemyStats.m_queueElemPos[index]);
      pPos.Z -= 1.0;
      ePos.Z -= 1.0;
      Utils.SetPosition((uint)P_QUEUE_HIGHLIGHT, pPos);
      Utils.SetPosition((uint)E_QUEUE_HIGHLIGHT, ePos);
    }

    static public bool IsResolutionPhase() {  return isResolutionPhase; }
  }
}
