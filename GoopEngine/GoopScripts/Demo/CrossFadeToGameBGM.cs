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
      Utils.FadeOutAudio("menu", 0.8f);
      Utils.FadeInAudio("caveFighting", 0.8f, 1.0f);
    }
	}
}
