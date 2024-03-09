using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
using GoopScripts.Mono;

namespace GoopScripts.Demo
{
	internal class CrossFadeToBGM : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.FadeOutAudio("caveFighting", 0.8f);
			Utils.FadeInAudio("menu", 0.8f, 1.0f);
		}
	}
}
