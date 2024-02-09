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
			Utils.CrossFadeAudio("caveFighting", 0.8f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.8f, 0.0f, 0.0f, 1.0f);
			Utils.CrossFadeAudio("CaveWithWaterDrops_Loop", 0.486f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			Utils.CrossFadeAudio("Fog", 0.753f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			Utils.TransitionToScene("MainMenu");
		}
	}
}
