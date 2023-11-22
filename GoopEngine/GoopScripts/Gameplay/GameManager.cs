using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class GameManager
  {
    //public double timebeforeaction;
    //private double currtime;
    //random rng;
    //cardmanager cardmanager;

    GameManager()
    {
      //timeBeforeAction = 0;
      //currTime = 0;
      //rng = new Random();
      //cardManager = new CardManager();
    }

    // function to allow c++ to edit the list of cards in cardmanager
    // this should use cardmanager's c++ interface function

    public void OnUpdate(double dt, Stats player, Stats enemy)
    {
      //currTime -= dt;
      //Console.WriteLine("DT: " + dt);
      Console.WriteLine("Player: " + player.m_health);
      Console.WriteLine("Enemy: " + enemy.m_health);

      //   if (currTime < 0)
      //{
      //     // Reset the timer and include overflow value
      //	currTime = timeBeforeAction - currTime;

      //     // Play cards in queue
      //     cardManager.Cards[(int)playerStats.m_cardQueue.First()].Play(ref playerStats, ref enemyStats);
      //     playerStats.m_cardQueue.RemoveAt(0);
      //     cardManager.Cards[(int)enemyStats.m_cardQueue.First()].Play(ref playerStats, ref enemyStats);
      //	enemyStats.m_cardQueue.RemoveAt(0);
      //     // Should probably record state here, such as win, lose and continue battle
      //     // Indicate cards being played to c++ too

      //	// Everyone add 1 card to queue from deck
      //	// Calls a c++ function to do the animations?
      //	// or c++ can check if queue size changed and then do the animations for it
      //	playerStats.m_cardQueue.Add(playerStats.m_deck[rng.Next(0, playerStats.m_deck.Count() - 1)]);
      //	enemyStats.m_cardQueue.Add(enemyStats.m_deck[rng.Next(0, enemyStats.m_deck.Count() - 1)]);
      //}
    }
  }
}
