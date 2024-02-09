using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
	internal class GameOverToMainMenu : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "MainMenu");
		}
	}
}
