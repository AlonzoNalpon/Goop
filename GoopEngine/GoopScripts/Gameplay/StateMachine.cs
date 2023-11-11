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

    public Phase currPhase;

    public void NextPhase()
    {
      currPhase = (Phase)((int)(currPhase + 1) % (int)Phase.TOTAL_PHASES);
    }
  }
}
