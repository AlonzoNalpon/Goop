/*!*********************************************************************
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

namespace GoopScripts.Gameplay
{
  public class GameManager : Entity, IOnDestroy
  {
    //static readonly double INTERVAL_TIME = 3.0;
    static readonly Vec3<double> ENEMY_POS = new Vec3<double>(336.318, 100.0, 0.0);
    static readonly string GAME_DATA_DIR = "./Assets/GameData/";
    public int PAUSE_MENU, HOWTOPLAY_MENU, QUIT_MENU;
    public int P_QUEUE_HIGHLIGHT, E_QUEUE_HIGHLIGHT;
    public int P_HEALTH_TEXT_UI, P_HEALTH_UI, E_HEALTH_TEXT_UI, E_HEALTH_UI;
    public int P_SKIPPED_UI, E_SKIPPED_UI;
    public double SKIP_TURN_DELAY;

    public Stats m_playerStats, m_enemyStats;

    static int m_currentLevel;
    static bool isResolutionPhase = false;
    static bool gameEnded = false;

    //tools for resolving cards
    int m_slotToResolve = 0;
    double m_timer;
    bool m_playerSkipped;
    static bool isStartOfTurn = true;
    static bool gameStarted = false; // called once at the start of game

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
      OnUpdate function for GameManager
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
          m_timer = 0.0;
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

        if (isResolutionPhase)
        {
          m_playerStats.Update(deltaTime);
          m_enemyStats.Update(deltaTime);
          ResolutionPhase(deltaTime);
        }
        else if (isStartOfTurn)
        {
          isStartOfTurn = false;
          StartOfTurn();
        }
      }
#if (DEBUG)
      catch (Exception ex)
      {
        Console.WriteLine($"Exception in game loop: {ex.Message}");
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
        This function is triggered when the start of turn occurs. Player and enemy will draw cards. Also triggers the Enemy's AI
      ************************************************************************/
    public void StartOfTurn()
    {
      QueueCardDisplay.DestroyCard();
      SetHighlightActive(false);
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      m_playerStats.Draw();
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
        This function handles the resolution phase. It plays the animation based on the cards played.
        It also trigger combos if player/enemy plays more than 1 card
      \param deltaTime
        delta time since last frame
      ************************************************************************/
    public void ResolutionPhase(double deltaTime)
    {
      // there are 3 actions to resolve and it should only run after
      // animation time. So our condition here would be the timer

      // only proceed if no animation is playing
      if (m_playerStats.IsPlayingAnimation() || m_enemyStats.IsPlayingAnimation())
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

          if (m_enemyStats.m_deckMngr.m_queue[m_slotToResolve+1].Item1 != CardBase.CardID.NO_CARD && m_enemyStats.m_deckMngr.m_queue[m_slotToResolve].Item1 != CardBase.CardID.NO_CARD)
          {
            ComboManager.Combo(ref m_enemyStats, ref m_playerStats, (m_slotToResolve - 1));
          }
        }
        HighlightQueueSlot(m_slotToResolve);
        ++m_slotToResolve;
        m_playerStats.ClearAtKBlk();
        m_enemyStats.ClearAtKBlk();
      }
      // if game has ended
      else
      {
        if (m_currentLevel == 2)
        {
          File.Copy(GAME_DATA_DIR + "DefaultStats.sav", GAME_DATA_DIR + "PlayerStats.sav", true);
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Victory");
        }

        if (m_playerStats.IsDead())
        {
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Defeat");
        }
        else
        {
          SerialReader.SavePlayerState(ref m_playerStats, m_currentLevel, GAME_DATA_DIR + "PlayerStats.sav");
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Reward");
        }
      }
    }


    /*!*********************************************************************
      \brief
        This function is triggered t the satrt of resolution phase. 
        This reset the variables used when resolving the resolution phase
      ************************************************************************/
    private void StartResolution()
    {
      SetHighlightActive(true);
    }


    /*!*********************************************************************
    \brief
      This funciton is triggered when the user clicks the end turn button. 
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

    static public void ResetGameManager()
    {
      isResolutionPhase = isStartOfTurn = gameStarted = gameEnded = false;
    }

    static public bool IsResolutionPhase() {  return isResolutionPhase; }

    void LoadGame(string filePath)
    {
      // Load player and enemy stats
      PlayerStatsInfo playerStats = Serialization.SerialReader.LoadPlayerState(filePath);
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

    void LoadPlayer(PlayerStatsInfo statsInfo)
    {
      m_playerStats.m_type = CharacterType.PLAYER; 
      foreach (var elem in statsInfo.deckList)
      {
        m_playerStats.m_deckMngr.m_deck.AddCard(elem.Item1, elem.Item2);
      }
      m_playerStats.m_deckMngr.Init();
      m_playerStats.m_healthBar.Init(statsInfo.health, statsInfo.maxHealth, true, P_HEALTH_TEXT_UI, P_HEALTH_UI);
    }
    
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
      Utils.SpawnPrefab(statsInfo.background, new Vec3<double>(0, 0, -999));
      Utils.UpdateSprite(GetEntity("Enemy Portrait"), statsInfo.portrait);
    }

		public void OnDestroy(uint entityid)
		{
      PauseManager.SetPauseState(0);
		}
	}
}
