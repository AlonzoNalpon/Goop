using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  public class MainMenuToScene : IButtonClick
  {
    public int Scene;
    public void OnClick(uint entity)
    {
      switch (Scene)
      {
        case 0:
					Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Cutscene");
					break;
        case 1: // LOAD GAME

          break;
        case 2:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Options");
          break;
        case 3:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "HowToPlay");
          break;
        case 4:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Credits");
          break;
        default: break;
      }
    }
  }
}
