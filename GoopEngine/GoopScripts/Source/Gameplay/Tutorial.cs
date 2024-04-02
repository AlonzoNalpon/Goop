/*!*********************************************************************
\file   Tutorial.cs
\author c.phua\@digipen.edu
\date   15-March-2024
\brief  Main update loop for the Tutorial scene. Triggers step by
        step instructions by setting relevant entities to active/inactive
        while using the same logic as the normal game scene's GameManager.

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
using GoopScripts.Serialization;
using GoopScripts.UI;

namespace GoopScripts.Gameplay
{
  public class Tutorial : Entity, IOnDestroy
  {
    static readonly Vec3<double> ENEMY_POS = new Vec3<double>(336.318, 112.0, 0.0);
    static readonly string GAME_DATA_DIR = "./Assets/GameData/";
    public int PAUSE_MENU, HOWTOPLAY_MENU, QUIT_MENU;
    public int P_QUEUE_HIGHLIGHT, E_QUEUE_HIGHLIGHT;
    public int P_HEALTH_TEXT_UI, P_HEALTH_UI;
    public int E_HEALTH_TEXT_UI, E_HEALTH_UI;

    static int m_turn;
    static public int m_tut;

    public Stats m_playerStats, m_enemyStats;

    static bool isResolutionPhase = false;
    static bool gameEnded = false;

    int m_slotToResolve = 0;
    static bool isStartOfTurn = true;
    static bool gameStarted = false; // called once at the start of game 

    //flags to toggle tutorial pop-ups
    static public bool m_tutorialToggled;
    Tutorial(uint entityID):base(entityID)
    {
      
    }

    public void OnCreate()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");

      // set the static variable to the entity holding the hover effect sprite
      SelectCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(SelectCard.m_cardHover, false);

      ResetTutorial();
			Utils.SetLoseFocus(false);
		}

    public void OnUpdate(double deltaTime)
    {
      try
      {
        //button scripts
        if (m_tut == 1 || m_tut == 10 || m_tut == 15 || m_tut == 19 || m_tut == 25 || m_tut == 30 || m_tut == 32 || m_tut == 38 || m_tut == 43)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), true);
        }
        else if (m_tut == 8 || m_tut == 12 || m_tut == 16 || m_tut == 24 || m_tut == 28 || m_tut == 31 || m_tut == 34 || m_tut == 42 || m_tut == 46)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), true);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), false);
        }
        else if (m_tut == 9 || m_tut == 13 || m_tut == 14 || m_tut == 17 || m_tut == 18 || m_tut == 29 || (m_tut >= 35 && m_tut <= 37) || m_tut == 47 || m_tut == 48)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), false);
        }
        else if ((m_tut >= 2 && m_tut <= 7) || m_tut == 11 || (m_tut >= 20 && m_tut <= 23) || m_tut == 26 || m_tut == 27 || m_tut == 33 || m_tut == 39 || m_tut == 40 || m_tut == 41 || m_tut == 44 || m_tut == 45)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), true);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), true);
        }

        //combo list
        if (!Utils.GetIsActiveEntity(Utils.GetEntity("Skip_Tutorial_Prompt")) && UI.PauseManager.GetPauseState() == 0)
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
          LoadGame(GAME_DATA_DIR + "TutorialStats.sav");
          for (int i = 0; i < 5; ++i)
          {
            m_playerStats.TutorialPlayerDraw(false);
          }
          m_enemyStats.Draw(false);
          m_enemyStats.QueueCard(0);
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

        if (m_tutorialToggled)
        {
          int m_prev = m_tut++;

          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_prev}"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_tut}"), true);

          m_tutorialToggled = false;
        }

        if (isResolutionPhase)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_tut}"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), false);

          m_playerStats.Update(deltaTime);
          m_enemyStats.Update(deltaTime);
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
      catch (Exception ex)
      {
        Console.WriteLine($"Exception in game loop: {ex.Message}");
      }
    }

    public void StartOfTurn()
    {
      QueueCardDisplay.DestroyCard();
      SetHighlightActive(false);
      m_playerStats.EndOfTurn();
      m_enemyStats.EndOfTurn();
      ++m_turn;
      m_playerStats.TutorialPlayerDraw();
      if (m_turn == 4)
      {
        m_enemyStats.Draw(false);
        m_enemyStats.Draw(false);
        m_enemyStats.QueueCard(0);
        m_enemyStats.QueueCard(0);
      }
      else
      {
        m_enemyStats.Draw(false);
        m_enemyStats.QueueCard(0);
      }
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{++m_tut}"), true); 
      Button.TutorialBtn btn = (Button.TutorialBtn)Utils.GetScript("Button_EndTurn", "TutorialBtn");
			btn.Enable();
		}

    public void ResolutionPhase(double deltaTime)
    {
      // only proceed if no animation is playing
      if (m_playerStats.IsPlayingAnimation() || m_enemyStats.IsPlayingAnimation())
      {
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

          if (m_enemyStats.m_deckMngr.m_queue[m_slotToResolve - 1].Item1 != CardBase.CardID.NO_CARD && m_enemyStats.m_deckMngr.m_queue[m_slotToResolve].Item1 != CardBase.CardID.NO_CARD)
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
        // in case
        if (m_playerStats.IsDead())
        {
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Defeat");
        }
        else
        {
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Reward");
        }
      }
    }

    /*!*********************************************************************
      \brief
        This function is triggered t the satrt of resolution phase. This reset the variables
        used when resolving the resolution phase
      ************************************************************************/
    private void StartResolution()
    {
      SetHighlightActive(true);
      Random rand = new Random();
      int randomNumber = rand.Next(100);
      if (randomNumber <= 25)
      {
        List<string> playerVoiceLine = new List<string> { "VL_Leah_Enemy_1", "VL_Leah_Enemy_2" };
        int voiceLineInd = rand.Next(playerVoiceLine.Count);
        PlaySoundF(playerVoiceLine[voiceLineInd], 1.0f, ChannelType.VOICE, false);
      }
    }

    /*!*********************************************************************
      \brief
        This funciton is triggered when the user clicks the end turn button. THis function
        starts the resolution phase
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

    static public void ResetTutorial()
    {
      isResolutionPhase = isStartOfTurn = gameStarted = m_tutorialToggled = gameEnded = false;
      m_turn = 1;
      m_tut = 0;
    }

    static public bool IsResolutionPhase() { return isResolutionPhase; }

    void LoadGame(string filePath)
    {
      PlayerStatsInfo playerStats = Serialization.SerialReader.LoadPlayerState(filePath);
      LoadPlayer(playerStats);

      string levelFile = GAME_DATA_DIR + "/Tutorial.dat";
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
