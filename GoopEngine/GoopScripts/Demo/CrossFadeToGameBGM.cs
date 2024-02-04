using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
using GoopScripts.Mono;

namespace GoopScripts.Demo
{
	internal class CrossFadeToGameBGM : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.CrossFadeAudio("menu", 0.8f, 0.0f, 0.0f, 0.8f, "caveFighting", 0.0f, 0.8f, 0.0f, 1.0f, 1.0f);
		}
	}
}
