using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
using GoopScripts.Mono;

namespace GoopScripts.Demo
{
	internal class StopBGM : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.StopChannel(Utils.ChannelType.BGM);
		}
	}
}
