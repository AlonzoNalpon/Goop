using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
	internal class GameOverToMainMenu : TextButtonBase
	{

    public override void OnRelease(uint entity)
    {
			base.OnRelease(entity);
			Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "MainMenu");
		}
  }
}
