using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;

namespace GoopScripts.Transition
{
	internal class ChangeToMainMenu : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.TransitionToScene("MainMenu");
		}
	}
}
