using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
  internal class StateMachine
  {
    public enum Phase
    {
      START = 0,
      PLAYER,
      ENEMY,
      RESOLUTION,
      END,
      TOTAL_PHASES
    }

    public double timeBeforeAction;
    private double currTime;
    public Phase currPhase;
    Random rng;

    StateMachine()
    {
      timeBeforeAction = 0;
      currTime = 0;
      currPhase = Phase.START;
      rng = new Random();
    }

    public void NextPhase()
		{
			// Should indicate to c++ to do next phase animations
			currPhase = (Phase)((int)(currPhase + 1) % (int)Phase.TOTAL_PHASES);
    }

    public void Update(Stats playerStats, Stats enemyStats, double dt)
    {
      currTime -= dt;

      if (currTime < 0)
			{
				currTime = timeBeforeAction;

        NextPhase();
        // Play cards in queue
        playerStats.cardQueue.First();

        // Everyone add 1 card to queue from deck
        playerStats.cardQueue.Add(playerStats.deck[rng.Next(0, playerStats.deck.Count() - 1)]);
				enemyStats.cardQueue.Add(enemyStats.deck[rng.Next(0, enemyStats.deck.Count() - 1)]);
			}
    }
  }
}
