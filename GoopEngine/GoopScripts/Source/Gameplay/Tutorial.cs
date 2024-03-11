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

namespace GoopScripts.Gameplay
{
  public class Tutorial : Entity
  {
    static readonly Vec3<double> ENEMY_POS = new Vec3<double>(336.318, 112.0, 0.0);
    public int PAUSE_MENU, HOWTOPLAY_MENU, QUIT_MENU;
    public int P_QUEUE_HIGHLIGHT, E_QUEUE_HIGHLIGHT;
    public int P_HEALTH_TEXT_UI, P_HEALTH_UI;
    public int E_HEALTH_TEXT_UI, E_HEALTH_UI;

    static int m_turn;
    static public int m_tut;

    Random m_rng;
    double m_currTime = 0.0;
    double m_animTime = 0.0;


    public Stats m_playerStats, m_enemyStats;

    static bool isResolutionPhase = false;
    static bool isDead = false;

    int m_slotNum = 0;
    bool toTrigger = false;
    static bool isStartOfTurn = true;
    List<CardBase.CardID> m_cardsPlayedP = new List<CardBase.CardID>();
    List<CardBase.CardID> m_cardsPlayedE = new List<CardBase.CardID>();
    static bool gameStarted = false; // called once at the start of game 
    List<CardBase.CardID> m_playerNonAtkCards = new List<CardBase.CardID> { CardID.LEAH_SHIELD, CardID.SPECIAL_SMOKESCREEN, CardID.SPECIAL_RAGE };
    List<CardBase.CardID> m_enemyNonAtkCards = new List<CardBase.CardID> { CardID.DAWSON_SHIELD, CardID.BASIC_SHIELD };

    //flags to toggle tutorial pop-ups
    static public bool m_tutorialToggled;
    Tutorial(uint entityID):base(entityID)
    {
      m_rng = new Random();
    }

    public void OnCreate()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
#if (DEBUG)
      Console.WriteLine(m_enemyStats.m_healthBar.m_maxHealth);
      Console.WriteLine(m_enemyStats.m_healthBar.m_health);
#endif

      // set the static variable to the entity holding the hover effect sprite
      SelectCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(SelectCard.m_cardHover, false);

      UI.PauseManager.SetPauseState(0);

      m_turn = 1;
      m_tut = 0;

      // set the static variable to the entity holding the hover effect sprite
      TutorialSelectCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(TutorialSelectCard.m_cardHover, false);
      isDead = false;
      ResetTutorial();
    }

    public void OnUpdate(double deltaTime)
    {
      try
      {
        //button scripts
        if (m_tut == 1 || m_tut == 11 || m_tut == 16 || m_tut == 20 || m_tut == 26 || m_tut == 31 || m_tut == 33 || m_tut == 39 || m_tut == 44)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), true);
        }
        else if (m_tut == 9 || m_tut == 13 || m_tut == 17 || m_tut == 25 || m_tut == 29 || m_tut == 32 || m_tut == 35 || m_tut == 43 || m_tut == 46)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), true);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), false);
        }
        else if (m_tut == 10 || m_tut == 14 || m_tut == 15 || m_tut == 18 || m_tut == 19 || m_tut == 30 || m_tut == 36 || m_tut == 37 || m_tut == 38 || m_tut == 47 || m_tut == 48)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Back"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("Tutorial_Button_Next"), false);
        }
        else if (m_tut == 2 || m_tut == 3 || m_tut == 4 || m_tut == 5 || m_tut == 6 || m_tut == 7 || m_tut == 8 || m_tut == 12 || m_tut == 21 || m_tut == 22 || m_tut == 23 || m_tut == 24 || m_tut == 27 || m_tut == 28 || m_tut == 34 || m_tut == 40 || m_tut == 41 || m_tut == 42 || m_tut == 45)
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
          LoadGame("./Assets/GameData/TutorialStats.sav");
          Console.WriteLine("Player draw");
          for (int i = 0; i < 5; ++i)
          {
            m_playerStats.TutorialPlayerDraw();
          }
          Console.WriteLine("Enemy draw");
          m_enemyStats.Draw();
          //Console.WriteLine("Enemy deck:");
          //for (int i = 0; i < m_enemyStats.m_deckMngr.m_deck.Size(); i++)
          //{
          //  Console.WriteLine(m_enemyStats.m_deckMngr.m_deck.m_cards[i]);
          //}
          //Console.WriteLine("Enemy hand:");
          //for (int i = 0; i < m_enemyStats.m_deckMngr.m_hand.Count; i++)
          //{
          //  Console.WriteLine(m_enemyStats.m_deckMngr.m_hand[i]);
          //}
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
          Console.WriteLine("tutorial toggled");
          int m_prev = m_tut++;

          Console.WriteLine($"Tutorial Number {m_tut}");
          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_prev}"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_tut}"), true);

          m_tutorialToggled = false;
        }

        if (isResolutionPhase)
        {
          //Console.WriteLine($"Tutorial Number {m_tut}");
          Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_tut}"), false);
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
        m_enemyStats.Draw();
        m_enemyStats.Draw();
        m_enemyStats.QueueCard(0);
        m_enemyStats.QueueCard(0);
      }
      else
      {
        m_enemyStats.Draw();
        m_enemyStats.QueueCard(0);
      }
      Console.WriteLine("StartOfTurn");
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{++m_tut}"), true); 
      Button.TutorialBtn btn = (Button.TutorialBtn)Utils.GetScript("Button_EndTurn", "TutorialBtn");
			btn.Enable();
		}

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
          if (toTriggerPlayerTakeDmg)
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
              Utils.PlayAnimation("SS_MoleRat_Flinch", m_enemyStats.entityID);
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
            deathTime = (Utils.GetAnimationTime("SS_Leah_Death") > deathTime) ? Utils.GetAnimationTime("SS_Leah_Death") : deathTime;
            isDead = true;

          }
          else if (m_enemyStats.IsDead())
          {
            Utils.PlayAnimation("SS_MoleRat_Death", m_enemyStats.entityID);
            deathTime = (Utils.GetAnimationTime("SS_MoleRat_Death") > deathTime) ? Utils.GetAnimationTime("SS_MoleRat_Death") : deathTime;
            isDead = true;
          }

          m_animTime = (deathTime > m_animTime) ? deathTime : m_animTime;
          m_playerStats.ClearAtKBlk();
          m_enemyStats.ClearAtKBlk();
        }

        HighlightQueueSlot(m_slotNum);
        ++m_slotNum;
      }
      else
      {
        if (m_currTime >= m_animTime)
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
      isResolutionPhase = isStartOfTurn = gameStarted = m_tutorialToggled = false;
      m_turn = 1;
      m_tut = 0;
    }

    static public bool IsResolutionPhase() { return isResolutionPhase; }

    void LoadGame(string filePath)
    {
      PlayerStatsInfo playerStats = Serialization.SerialReader.LoadPlayerState(filePath);
      LoadPlayer(playerStats);

      string levelFile = "./Assets/GameData/Tutorial.dat";
      LoadEnemy(Serialization.SerialReader.LoadEnemy(levelFile));
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

      //m_enemyStats.m_type = statsInfo.characterType;
      m_enemyStats.m_deckMngr.Clear();
      m_enemyStats.m_deckMngr.m_deck.m_cards = new CardID[0];
      foreach (var elem in statsInfo.deckList)
      {
        m_enemyStats.m_deckMngr.m_deck.AddCard(elem.Item1, elem.Item2);
      }

      m_enemyStats.m_healthBar.Init(statsInfo.health, statsInfo.maxHealth, false, E_HEALTH_TEXT_UI, E_HEALTH_UI);
      Utils.UpdateSprite(GetEntity("Background"), statsInfo.background);
      Utils.UpdateSprite(GetEntity("Enemy Portrait"), statsInfo.portrait);
    }
  }
}
