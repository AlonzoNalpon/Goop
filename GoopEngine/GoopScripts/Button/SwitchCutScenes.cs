using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Audio;
using GoopScripts.Button;
using GoopScripts.Demo;
using GoopScripts.Mono;
using GoopScripts.Transition;

namespace GoopScripts.Cutscene
{
	internal class SwitchCutScenes : IButtonClick
	{
		int frame;

		SwitchCutScenes()
		{
			frame = 1;
		}

		public void OnClick(uint entity)
		{
			int lastFrame = frame++;

			// 7 is the current number of cutscenes
			if (frame >= 9)
			{
				// Call existing function
				CrossFadeToGameBGM temp = new CrossFadeToGameBGM();
				temp.OnClick(entity);
				Utils.FadeInAudio("CaveWithWaterDrops_Loop", 0.486f, 1.0f);
				Utils.FadeInAudio("Fog", 0.753f, 1.0f);
				Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "TransitionOut");
			}
			else
			{
				Utils.SetIsActiveEntity(Utils.GetEntity($"Cutscenes_00{lastFrame}"), false);
				Utils.SetIsActiveEntity(Utils.GetEntity($"Cutscenes_00{frame}"), true);
			}

		}
	}
}
