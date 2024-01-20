using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Gameplay
{
  public class GameManager : MonoBehaviour
  {
    Random m_rng;
    double m_animTime = 1.0; // hard coded for now
    double m_currTime;

    Stats m_playerStats, m_enemyStats;

    bool endTurn = false;  // flag for triggering a turn

    GameManager()
    {
      m_rng = new Random();
    }

    public void OnCreate()
    {
      Console.WriteLine("Create GameManager");
      m_playerStats = (Stats) Utils.GetScript("Player", "Stats");
      m_enemyStats = (Stats)Utils.GetScript("Enemy", "Stats");
    }

    // function to allow c++ to edit the list of cards in cardmanager
    // this should use cardmanager's c++ interface function
    public void OnUpdate(double dt, Stats player, uint playerEntity, Stats enemy, uint enemyEntity, uint playerHand, uint playerQueue, uint enemyQueue)
		{
      if (endTurn)
			{
        player.EndOfTurn();
        enemy.EndOfTurn();
        player.m_deckMngr.Draw();
        enemy.m_deckMngr.Draw();
        endTurn = false;
        //m_numResolves = 1;
				//m_numResolves = player.m_cardQueue.Length >= enemy.m_cardQueue.Length ? player.m_cardQueue.Length : enemy.m_cardQueue.Length;

				//// Do 1 turn of stuff
				//m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
        //m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

        //Utils.PlaySound(m_rng.Next(1, 1), playerEntity);
        //Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        //Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
        //Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        ////play anim?
        //Utils.PlayAnimation("SS_LeahShoot", playerEntity);

        //// remove card
        //player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        //enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        //++m_currResolves;
      }

      bool shouldEnd = true;
      if (m_currTime > m_animTime)
      {
        // Overflow the time
        m_currTime = m_animTime - m_currTime;

				// Do 1 turn of stuff
				//m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
				//m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

        //play anim? play sound?
        Utils.PlaySound(m_rng.Next(0, 1), playerEntity);
        //Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
        //Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);

        // remove card
        //player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        //enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;

    //    ++m_currResolves;

				//if (m_currResolves >= m_numResolves)
				//{
				//	shouldEnd = true;
				//}
				Utils.PlayAnimation("SS_LeahShoot", playerEntity);
			}
      else
      {
				m_currTime += dt;
			}

      if (shouldEnd)
			{
				// Draw cards here.        
				Utils.GameSystemResolved();
        //int i = 0;
        //for (; i < player.m_hand.Length; ++i)
        //{
        //  if (player.m_hand[i] == CardBase.CardID.NO_CARD)
        //  {
        //    break;
        //  }
        //}
        //// Draw card if there is space, if no space burn card
        //if (i < player.m_hand.Length && player.m_deck.Length > 0)
        //{
        //  int drawnCard = m_rng.Next(0, player.m_deck.Length - 1);

        //  List<CardBase.CardID> temp = player.m_deck.ToList();
        //  temp.Remove((CardBase.CardID)drawnCard);
        //  player.m_deck = temp.ToArray();

        //  Utils.SetHandCardID(playerHand, i, (int)player.m_deck[drawnCard]);
        //  player.m_hand[i] = player.m_deck[drawnCard];
        //}
        //m_currResolves = 0;
        m_currTime = 0;

				Utils.PlayAnimation("SS_LeahIdle", playerEntity);
			}

      if (Utils.IsKeyTriggered(Input.KeyCode.MOUSE_BUTTON_1))
      { 
        Console.WriteLine("MOUSE CLICK 1");
        // Utils.SetIsActiveEntity(0, false);
        Utils.SpawnPrefab("TestButton", new Vec3<double>( Utils.GetMouseScrollX(), Utils.GetMouseScrollY(), 0 ));
      }
    }

    public void StartOfTurn()
    {

    }

    public void EndTurn()
    {
      Console.WriteLine("Current Queue:");

      foreach (CardBase.CardID c in m_playerStats.m_deckMngr.m_queue)
      {
        Console.WriteLine(c.ToString());
      }

      endTurn = true;
    }
  }
}
