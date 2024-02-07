using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  internal class ChangeToGameScene : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.TransitionToScene("Game");
    }
  }
}
