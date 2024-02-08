using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
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
				Utils.CrossFadeAudio("menu", 0.9f, 0.0f, 0.0f, 0.9f, "caveFighting", 0.0f, 0.8f, 0.1f, 1.0f, 1.0f);
				Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "CaveWithWaterDrops_Loop", 0.0f, 0.486f, 0.2f, 1.0f, 1.0f);
				Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "Fog", 0.0f, 0.753f, 0.2f, 1.0f, 1.0f);
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
