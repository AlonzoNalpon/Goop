using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Audio
{
	internal class StopGameAudio : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.CrossFadeAudio("caveFighting", 0.8f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.8f, 0.0f, 0.0f, 1.0f);
			Utils.CrossFadeAudio("CaveWithWaterDrops_Loop", 0.486f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			Utils.CrossFadeAudio("Fog", 0.753f, 0.0f, 0.0f, 0.9f, "", 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}
