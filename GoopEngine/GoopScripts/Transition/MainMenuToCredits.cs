using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  public class MainMenuToCredits : IButtonClick
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
      //Utils.CrossFadeAudio("menu", 0.8f, 0.0f, 0.0f, 0.9f, "menu", 0.0f, 0.8f, 0.1f, 1.0f, 2.0f);
      //Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "CaveWithWaterDrops_Loop", 0.0f, 0.486f, 0.2f, 1.0f, 2.0f);
      //Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "Fog", 0.0f, 0.753f, 0.2f, 1.0f, 2.0f);
      //Utils.PlayTransformAnimation(Utils.GetEntity("Transition"), "Credits");
    }
  }
}
