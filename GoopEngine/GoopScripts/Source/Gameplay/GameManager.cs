﻿/*!*********************************************************************
\file       GameManager.cs
\author     chengen.lau\@digipen.edu
\co-author  c.phua\@digipen.edu, han.q@digipen.edu
\date       10-January-2024
\brief      The main update loop for the game scene. A single entity
            "GameSystem" will run this script in the scene.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
using System.IO;
using GoopScripts.Serialization;
using GoopScripts.UI;
using GoopScripts.Demo;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Rebar;


namespace GoopScripts.Gameplay
{
  public class GameManager : Entity, IOnDestroy
  {
    //static readonly double INTERVAL_TIME = 3.0;
    static readonly Vec3<double> ENEMY_POS = new Vec3<double>(336.318, 100.0, 0.0);
    static readonly string GAME_DATA_DIR = ".\\Assets\\GameData\\";
    static readonly double PLAYER_DRAW_ANIM_TIME = 0.6;
    static readonly double HEAL_ANIM_TIME = 2.0;

    public int PAUSE_MENU, HOWTOPLAY_MENU, QUIT_MENU;
    public int P_QUEUE_HIGHLIGHT, E_QUEUE_HIGHLIGHT;
    public int P_HEALTH_TEXT_UI, P_HEALTH_UI, E_HEALTH_TEXT_UI, E_HEALTH_UI;
    public int P_SKIPPED_UI, E_SKIPPED_UI;
    public double SKIP_TURN_DELAY;

    public Stats m_playerStats, m_enemyStats;

    static int m_currentLevel;
    static bool isResolutionPhase = false;
    static bool isDrawingCard = false;
    static bool gameEnded = false;

    //tools for resolving cards
    int m_slotToResolve = 0;
    double m_timer;
    static double m_playerDrawTimer;
    bool m_playerSkipped = false, firstTurn = true;
    static bool isStartOfTurn = true;
    static bool gameStarted = false; // called once at the start of game
    static bool shouldApplyChargeUp = false;

    GameManager(uint entityID):base(entityID)
    {
      
    }

    /*!*********************************************************************
    \brief
      OnCreate function for GameManager
    ************************************************************************/
    public void OnCreate()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");

      m_playerStats.m_deckMngr.m_deck.Shuffle();

      // set the static variable to the entity holding the hover effect sprite
      SelectCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(SelectCard.m_cardHover, false);
      ResetGameManager();
			Utils.SetLoseFocus(false);
		}


    /*!*********************************************************************
    \brief
      OnUpdate function for GameManager. Runs the behaviour for the game
      loop to trigger the resolution phase when the player ends the turn.
    \param deltaTime
      delta time since last frame
    ************************************************************************/
    public void OnUpdate(double deltaTime)
    {
      try
      {
        if (UI.PauseManager.GetPauseState() == 0)
        {
          if (Utils.IsKeyHeld(Input.KeyCode.TAB))
          {
            Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), true);
          }

          if (Utils.IsKeyReleased(Input.KeyCode.TAB))
          {
            Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), false);
          }
        }
        if (!gameStarted)
        {
          LoadGame(GAME_DATA_DIR + "/PlayerStats.sav");
          m_playerStats.Init();
          m_enemyStats.Init();
          gameStarted = true;
          m_playerSkipped = false;
          m_timer = m_playerDrawTimer = 0.0;
          switch (m_enemyStats.m_type)
          {
            case CharacterType.DAWSON_MINI:
              {
                PlayRandomSound(27, 27, m_enemyStats.entityID);
                break;
              }
            case CharacterType.DAWSON_FINAL:
              {
                PlayRandomSound(32, 32, m_enemyStats.entityID);
                break;
              }
          }

          m_playerStats.m_healthBar.AnimatedHeal((int)((m_playerStats.m_healthBar.m_maxHealth - m_playerStats.m_healthBar.m_health) * 0.5), HEAL_ANIM_TIME);
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
                Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), false);
                Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Left"), false);
              if (Utils.GetIsActiveEntity((uint)QUIT_MENU))
                Utils.UndeeperPause(PAUSE_MENU, QUIT_MENU);
              break;
            default:
              break;
          }
        }

        // CHEAT CODES
        if (Utils.IsKeyTriggered(Input.KeyCode.U))
        {
          // damage enemy
          m_enemyStats.m_healthBar.DecreaseHealth(1);
        }
        if (Utils.IsKeyTriggered(Input.KeyCode.I))
        {
          // heal player
          m_playerStats.m_healthBar.IncreaseHealth(1);
        }
        if (Utils.IsKeyTriggered(Input.KeyCode.O))
        {
          // draw card
          m_playerStats.Draw();
        }

        m_playerStats.Update(deltaTime);
        m_enemyStats.Update(deltaTime);
        m_playerStats.m_healthBar.Update(deltaTime);

        if (isResolutionPhase)
        {
          ResolutionPhase(deltaTime);
        }
        else if (isStartOfTurn)
        {
          isStartOfTurn = false;
          StartOfTurn();
        }

        if (isDrawingCard)
        {
          if (firstTurn)
          {
            firstTurn = false;
            isDrawingCard = false;
          }
          else
          {
            m_playerDrawTimer += deltaTime;

            if (m_playerDrawTimer > PLAYER_DRAW_ANIM_TIME)
            {
              isDrawingCard = false;
              m_playerDrawTimer = 0.0;
            }
          }
        }

      }
#if (DEBUG)
      catch (Exception ex)
      {
        Utils.SendString($"Exception in game loop: {ex.Message}");
      }
#else
      catch (Exception)
      {
        Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "MainMenu");
      }
#endif
    }

    /*!*********************************************************************
    \brief
      This function is triggered when the start of turn occurs. Player and
      enemy will draw cards. Also triggers the Enemy's AI.
    ************************************************************************/
    public void StartOfTurn()
    {
      QueueCardDisplay.DestroyCard();
      SetHighlightActive(false);
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      m_playerStats.PlayerDraw();
      PlayerDraw();
      m_enemyStats.Draw();
      if (!m_enemyStats.IsTurnSkipped())
      {
        StartAI(m_enemyStats.entityID);
      }
      else
      {
        Utils.PlayAllTweenAnimation((uint)E_SKIPPED_UI, "FloatUp");
      }
			Button.EndTurn btn = (Button.EndTurn)Utils.GetScript("Button_EndTurn", "EndTurn");
      btn.Enable();
      if (m_playerStats.IsTurnSkipped())
      {
        Utils.PlayAllTweenAnimation((uint)P_SKIPPED_UI, "FloatUp");
        m_playerSkipped = true;
        EndTurn();
      }
    }



    /*!*********************************************************************
      \brief
        This function handles the resolution phase. It plays the animation
        based on the cards played. It also trigger combos if player/enemy
        plays more than 1 card.
      \param deltaTime
        delta time since last frame
      ************************************************************************/
    public void ResolutionPhase(double deltaTime)
    {
      // there are 3 actions to resolve and it should only run after
      // animation time. So our condition here would be the timer

      // only proceed if no animation is playing
      if (m_playerStats.IsPlayingAnimation() || m_enemyStats.IsPlayingAnimation() || m_playerStats.m_healthBar.IsHealing())
      {
        return;
      }
      else if (m_playerSkipped)
      {
        m_timer += deltaTime;
        if (m_timer >= SKIP_TURN_DELAY)
        {
          m_timer = 0.0;
          m_playerSkipped = false;
        }
        return;
      }

      if (m_slotToResolve > 2)
      {
        isResolutionPhase = false;
        isStartOfTurn = true;
        m_slotToResolve = 0;
        return;
      }

      bool playerPlayedCard = false, enemyPlayedCard = false;
      CardBase playerCard = CardManager.Get(m_playerStats.m_deckMngr.m_queue[m_slotToResolve].Item1);
      CardBase enemyCard = CardManager.Get(m_enemyStats.m_deckMngr.m_queue[m_slotToResolve].Item1);
      
      if (!gameEnded)
      {
        // play player's and enemy's card
        if (playerCard.ID != CardBase.CardID.NO_CARD)
        {
          playerCard.Play(ref m_playerStats, ref m_enemyStats);
          m_playerStats.PlayAnimation(playerCard.ID);
          playerPlayedCard = true;
        }

        if (enemyCard.ID != CardBase.CardID.NO_CARD)
        {
          enemyCard.Play(ref m_enemyStats, ref m_playerStats);
          m_enemyStats.PlayAnimation(enemyCard.ID);
          enemyPlayedCard = true;
        }

        int pCalculatedDmg = m_playerStats.DamageDealt(m_slotToResolve), eCalculatedDmg = m_enemyStats.DamageDealt(m_slotToResolve);
        int pDamageTaken = m_playerStats.TakeDamage(eCalculatedDmg, m_slotToResolve), eDamageTaken = m_enemyStats.TakeDamage(pCalculatedDmg, m_slotToResolve);

        // if any side is dead, end the game loop
        if (m_playerStats.IsDead())
        {
          if (playerCard.Type == CardType.BLOCK)
          {
            m_playerStats.m_animManager.PlayShieldDeath();
          }
          else
          {
            m_playerStats.m_animManager.PlayDeath();
          }
          gameEnded = true;
          return;
        }
        else if (m_enemyStats.IsDead())
        {
          if (enemyCard.Type == CardType.BLOCK)
          {
            m_enemyStats.m_animManager.PlayShieldDeath();
          }
          else
          {
            m_enemyStats.m_animManager.PlayDeath();
          }
          gameEnded = true;
          Random rand = new Random();
          int charrandomNumber = rand.Next(10);
          if (charrandomNumber < 5)
          {
            switch (m_enemyStats.m_type)
            {
              case CharacterType.DAWSON_MINI:
                {
                  PlayRandomSound(26, 26, m_enemyStats.entityID);
                  break;
                }
              case CharacterType.DAWSON_FINAL:
                {
                  PlayRandomSound(31, 31, m_enemyStats.entityID);
                  break;
                }
            }
          }
          else
          {
            List<string> playerVoiceLine = new List<string>();
            if (m_enemyStats.m_type == CharacterType.DAWSON_MINI)
            {
              playerVoiceLine = new List<string> { "VL_Leah_MiniBossDeath" };
            }
            else
            {
              playerVoiceLine = new List<string> { "VL_Leah_EnemyDeath" };
            }
            int voiceLineInd = rand.Next(playerVoiceLine.Count);
            PlaySoundF(playerVoiceLine[voiceLineInd], 1.0f, ChannelType.VOICE, false);
          }
          return;
        }

        // if enemy attacked, play relevant animation for taking damage
        if (!playerPlayedCard && enemyCard.Type == CardType.ATTACK)
        {
          m_playerStats.PlayDamagedAnimation(pDamageTaken);
        }

        // if player attacked, play relevant animation for taking damage
        if (!enemyPlayedCard && playerCard.Type == CardType.ATTACK)
        {
          m_enemyStats.PlayDamagedAnimation(eDamageTaken);
        }

        // check whether a combo needs to be resolved
        if (m_slotToResolve > 0)
        {
          if (m_playerStats.m_deckMngr.m_queue[m_slotToResolve - 1].Item1 != CardBase.CardID.NO_CARD && m_playerStats.m_deckMngr.m_queue[m_slotToResolve].Item1 != CardBase.CardID.NO_CARD)
          {
            ComboManager.Combo(ref m_playerStats, ref m_enemyStats, (m_slotToResolve - 1));
          }

          if (m_enemyStats.m_deckMngr.m_queue[m_slotToResolve - 1].Item1 != CardBase.CardID.NO_CARD && m_enemyStats.m_deckMngr.m_queue[m_slotToResolve].Item1 != CardBase.CardID.NO_CARD)
          {
            ComboManager.Combo(ref m_enemyStats, ref m_playerStats, (m_slotToResolve - 1));
          }
        }
        HighlightQueueSlot(m_slotToResolve);
        ++m_slotToResolve;

        // apply chargeup buff if needed
        if (m_slotToResolve > 2 && shouldApplyChargeUp)
        {
          m_enemyStats.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 1));
          m_enemyStats.m_buffs.AddBuff(new Buff(Buff.BuffType.FLAT_ATK_UP, 2.0f, 2));
          shouldApplyChargeUp = false;
        }

        m_playerStats.ClearAtKBlk();
        m_enemyStats.ClearAtKBlk();
      }
      // if game has ended
      else
      {
        if (m_playerStats.IsDead())
        {
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Defeat");
        }
        else
        {
         
          
          if (m_currentLevel == 4)
          {
            SerialReader.GenerateDefaultSave();
            Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Victory");
          }
          else
          {
            SerialReader.SavePlayerState(ref m_playerStats, m_currentLevel);
            Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Reward");
          }
        }
      }
    }


    /*!*********************************************************************
    \brief
      This function is triggered at the satrt of resolution phase. 
      This reset the variables used when resolving the resolution phase
    ************************************************************************/
    private void StartResolution()
    {
      SetHighlightActive(true);
      Random rand = new Random();
      int randomNumber = rand.Next(100);
      if(randomNumber <= 25)
      {
        int charrandomNumber = rand.Next(10);
        if(charrandomNumber <= 5)
        {
          switch (m_enemyStats.m_type)
          {
            case CharacterType.DAWSON_MINI:
              {
                PlayRandomSound(28, 30, m_enemyStats.entityID);
                break;
              }
            case CharacterType.DAWSON_FINAL:
              {
                PlayRandomSound(23, 25, m_enemyStats.entityID);
                break;
              }
          }
        }

        else
        {
          List<string> playerVoiceLine = new List<string>();
          if (m_enemyStats.m_type == CharacterType.DAWSON_MINI || m_enemyStats.m_type == CharacterType.DAWSON_FINAL)
          {
            playerVoiceLine = new List<string> { "VL_Leah_BossFight_1", "VL_Leah_BossFight_2", "VL_Leah_BossFight_3" };
          }
          else
          {
            playerVoiceLine = new List<string> { "VL_Leah_Enemy_1", "VL_Leah_Enemy_2" };
          }

          int voiceLineInd = rand.Next(playerVoiceLine.Count);
          PlaySoundF(playerVoiceLine[voiceLineInd], 1.0f, ChannelType.VOICE,false);
        }
        
      }
    }


    /*!*********************************************************************
    \brief
      This function is triggered when the user clicks the end turn button. 
      This function starts the resolution phase
    ************************************************************************/
    public void EndTurn()
    {
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

    /*!*********************************************************************
    \brief
      Resets the game manager to its default values
    ************************************************************************/
    static public void ResetGameManager()
    {
      isResolutionPhase = isStartOfTurn = gameStarted = gameEnded = false;
    }

    /*!*********************************************************************
    \brief
      Checks if the game is currently in the resolution phase
    \return
      True if the game is in resolution phase and false otherwise
    ************************************************************************/
    static public bool IsResolutionPhase() {  return isResolutionPhase; }

    /*!*********************************************************************
    \brief
      Checks if the player's draw animation is playing
    \return
      True if the player is currently drawing a card and false otherwise
    ************************************************************************/
    static public bool IsDrawingCard() { return isDrawingCard; }

    /*!*********************************************************************
    \brief
      Loads the player's health and deck from the save file and loads the
      corresponding enemy based on the level.
      Additionally, the relevant animations of the player and enemy are
      also read from a separate file and loaded accordingly.
    \param filePath
      The path of the save file
    ************************************************************************/
    void LoadGame(string filePath)
    {
      // Load player and enemy stats
      PlayerStatsInfo playerStats = Serialization.SerialReader.LoadPlayerState();
      LoadPlayer(playerStats);

      m_currentLevel = playerStats.levelToLoad;
      if (m_currentLevel < 0)
      {
        Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Tutorial");
      } 
      else if (m_currentLevel == 0)
      {
        m_currentLevel = 1;
      }
      string levelFile = GAME_DATA_DIR + "Level" + m_currentLevel + ".dat";
      LoadEnemy(Serialization.SerialReader.LoadEnemy(levelFile));

      // Load their animations
      var charToAnims = Serialization.SerialReader.LoadAnimationMappings(GAME_DATA_DIR + "CharacterAnimations.dat");
      m_playerStats.m_animManager.LoadAnimations(charToAnims[CharacterType.PLAYER]);
      m_enemyStats.m_animManager.LoadAnimations(charToAnims[m_enemyStats.m_type]);
    }

    /*!*********************************************************************
    \brief
      Loads the player's deck and initializes its healthbar
    \param statsInfo
      The player's information contained in a PlayerStatsInfo object
    ************************************************************************/
    void LoadPlayer(PlayerStatsInfo statsInfo)
    {
      m_playerStats.m_type = CharacterType.PLAYER; 
      foreach (var elem in statsInfo.deckList)
      {
        m_playerStats.m_deckMngr.m_deck.AddCard(elem.Item1, elem.Item2);
      }
      m_playerStats.m_deckMngr.Init();
      m_playerStats.m_buffs.SetType(CharacterType.PLAYER);
      m_playerStats.m_healthBar.Init(statsInfo.health, statsInfo.maxHealth, true, P_HEALTH_TEXT_UI, P_HEALTH_UI);
    }

    /*!*********************************************************************
    \brief
      Creates the enemy by spawning its prefab and initializes its deck and
      healthbar. The enemy portrait and background is also loaded here.
    \param statsInfo
      The level's information contained in a EnemyStatsInfo object
    ************************************************************************/
    void LoadEnemy(EnemyStatsInfo statsInfo)
    {
      uint enemyID = Utils.SpawnPrefab(statsInfo.prefab, ENEMY_POS);
      Utils.SetEntityName(enemyID, "Enemy");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
      m_enemyStats.OnCreate();

			m_enemyStats.m_type = statsInfo.characterType;
			m_enemyStats.m_deckMngr.Clear();
			m_enemyStats.m_deckMngr.m_deck.m_cards = new CardID[0];
			foreach (var elem in statsInfo.deckList)
      {
        m_enemyStats.m_deckMngr.m_deck.AddCard(elem.Item1, elem.Item2);
      }
      m_enemyStats.m_deckMngr.Init();
      m_enemyStats.m_healthBar.Init(statsInfo.health, statsInfo.maxHealth, false, E_HEALTH_TEXT_UI, E_HEALTH_UI);
      m_enemyStats.m_buffs.SetType(m_enemyStats.m_type);
      Utils.SpawnPrefab(statsInfo.background, new Vec3<double>(0, 0, -999));
      Utils.UpdateSprite(GetEntity("Enemy Portrait"), statsInfo.portrait);
    }

    /*!*********************************************************************
    \brief
      Sets the flag to apply charge up buff on enemy
    ************************************************************************/
    static public void ApplyChargeUp()
    {
      shouldApplyChargeUp = true;
    }

    /*!*********************************************************************
    \brief
      Sets the flag and resets the timer for the player draw timer
    ************************************************************************/
    static public void PlayerDraw()
    {
      isDrawingCard = true;
      m_playerDrawTimer = 0.0;
    }

    /*!*********************************************************************
    \brief
      Resets the pause state of the game when the game scene is destroyed
    ************************************************************************/
    public void OnDestroy(uint entityid)
		{
      PauseManager.SetPauseState(0);
		}
	}
}
