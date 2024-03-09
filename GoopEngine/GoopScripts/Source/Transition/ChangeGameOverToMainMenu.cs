using GoopScripts.Audio;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
	internal class ChangeGameOverToMainMenu : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			// Hack here and just invoke the stop game audio onclick event
			StopGameAudio temp = new StopGameAudio();
			temp.OnClick(entity);
			Utils.TransitionToScene("MainMenu");
		}
	}
}
