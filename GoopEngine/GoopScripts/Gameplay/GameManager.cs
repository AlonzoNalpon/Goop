using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
  internal class GameManager
  {
    public double timeBeforeAction;
    private double currTime;
    Random rng;
    CardManager cardManager;

		GameManager()
    {
      timeBeforeAction = 0;
      currTime = 0;
      rng = new Random();
      cardManager = new CardManager();
    }

    public void Update(Stats playerStats, Stats enemyStats, double dt)
    {
      currTime -= dt;

      if (currTime < 0)
			{
        // Reset the timer and include overflow value
				currTime = timeBeforeAction - currTime;

        // Play cards in queue
        cardManager.Cards[(int)playerStats.cardQueue.First()].Play();
        playerStats.cardQueue.RemoveAt(0);
        cardManager.Cards[(int)enemyStats.cardQueue.First()].Play();
				enemyStats.cardQueue.RemoveAt(0);
        // Should probably record state here, such as win, lose and continue battle
        // Indicate cards being played to c++ too

				// Everyone add 1 card to queue from deck
				// Calls a c++ function to do the animations?
				// or c++ can check if queue size changed and then do the animations for it
				playerStats.cardQueue.Add(playerStats.deck[rng.Next(0, playerStats.deck.Count() - 1)]);
				enemyStats.cardQueue.Add(enemyStats.deck[rng.Next(0, enemyStats.deck.Count() - 1)]);
			}
    }
  }
}
