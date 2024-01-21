using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;

namespace GoopScripts.Audio
{
	internal class MainMenuToGame : IOnDestroy
	{
		public void OnDestroy(uint entityid)
		{
			Utils.CrossFadeAudio("menu", 0.8f, 0.0f, 0.0f, 0.9f, "caveFighting", 0.0f, 0.8f, 0.1f, 1.0f, 2.0f);
			Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "CaveWithWaterDrops_Loop", 0.0f, 0.486f, 0.2f, 1.0f, 2.0f);
			Utils.CrossFadeAudio("", 0.0f, 0.0f, 0.0f, 0.0f, "Fog", 0.0f, 0.753f, 0.2f, 1.0f, 2.0f);
		}
	}
}
