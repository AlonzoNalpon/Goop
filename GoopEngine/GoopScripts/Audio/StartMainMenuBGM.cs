using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Audio
{
	internal class StartMainMenuBGM
	{
		public void OnCreate()
		{
			Utils.CrossFadeAudio("", 0, 0, 0, 0, "menu", 0, 0.9f, 0, 1, 1);
		}
	}
}
