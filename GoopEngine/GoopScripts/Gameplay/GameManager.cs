using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Mono;

namespace GoopScripts.Gameplay
{
  public class GameManager
  {
    Random m_rng;
    CardManager m_cardManager;
    double m_animTime = 1.0; // hard coded for now
    double m_currTime;
    int m_numResolves;
    int m_currResolves = 0;
    bool newTurn = true;

    GameManager()
    {
      m_rng = new Random();
      m_cardManager = new CardManager();
    }

    // function to allow c++ to edit the list of cards in cardmanager
    // this should use cardmanager's c++ interface function

    public void OnUpdate(double dt, Stats player, uint playerEntity, Stats enemy, uint enemyEntity)
    //public void OnUpdate(double dt, Stats player, Stats enemy)
		{
			Console.WriteLine("Updating");
			if (newTurn)
			{
				m_numResolves = player.m_cardQueue.Length >= enemy.m_cardQueue.Length ? player.m_cardQueue.Length : enemy.m_cardQueue.Length;
        newTurn = false;

				Console.WriteLine("Player health " + player.m_health);
				Console.WriteLine("Enemy health " + enemy.m_health);

				Console.WriteLine("Player queue size " + player.m_cardQueue.Length);
				Console.WriteLine("Enemy queue size " + enemy.m_cardQueue.Length);
				// Do 1 turn of stuff
				m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
        m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

        Utils.PlaySound(m_rng.Next(0, 1), playerEntity);
				Console.WriteLine("Player played card " + player.m_cardQueue[m_currResolves]);
        Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
        Console.WriteLine("Enemy played card " + enemy.m_cardQueue[m_currResolves]);
        //play anim?
        ++m_currResolves;
			}

			bool shouldEnd = false;
      if (m_currTime > m_animTime)
      {
        // Overflow the time
        m_currTime = m_animTime - m_currTime;

        Console.WriteLine(m_currResolves);
				// Do 1 turn of stuff
				m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
				m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

				Console.WriteLine("Player played card " + player.m_cardQueue[m_currResolves]);
				Console.WriteLine("Enemy played card " + enemy.m_cardQueue[m_currResolves]);
				//play anim? play sound?
				++m_currResolves;

				if (m_currResolves >= m_numResolves)
				{
					shouldEnd = true;
				}
      }
      else
      {
				m_currTime += dt;
			}

      if (shouldEnd)
			{
        // Draw cards here.
        Console.WriteLine("Turn end");

				Utils.GameSystemResolved();
        m_currResolves = 0;
        m_currTime = 0;
        newTurn = true;
			}      


      //// Play cards in queue
      //cardManager.Cards[(int)playerStats.m_cardQueue.First()].Play(ref playerStats, ref enemyStats);
      //playerStats.m_cardQueue.RemoveAt(0);
      //cardManager.Cards[(int)enemyStats.m_cardQueue.First()].Play(ref playerStats, ref enemyStats);
      //enemyStats.m_cardQueue.RemoveAt(0);
      //// Should probably record state here, such as win, lose and continue battle
      //// Indicate cards being played to c++ too

      //// Everyone add 1 card to queue from deck
      //// Calls a c++ function to do the animations?
      //// or c++ can check if queue size changed and then do the animations for it
      //playerStats.m_cardQueue.Add(playerStats.m_deck[rng.Next(0, playerStats.m_deck.Count() - 1)]);
      //enemyStats.m_cardQueue.Add(enemyStats.m_deck[rng.Next(0, enemyStats.m_deck.Count() - 1)]);
    }
  }
}
