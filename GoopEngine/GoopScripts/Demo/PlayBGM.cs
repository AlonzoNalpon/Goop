using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
using GoopScripts.Mono;

namespace GoopScripts.Demo
{
	internal class PlayBGM : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.PlaySoundF("menu", 0.8f, Utils.ChannelType.BGM, true);
		}
	}
}
