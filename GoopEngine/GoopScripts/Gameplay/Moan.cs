using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Gameplay
{
	internal class Moan : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.PlaySoundF("damageTaken_Leah", 1.0f, Utils.ChannelType.SFX, false);
			Utils.PlayAnimation("SS_Leah_FlashBang", entity);
		}
	}
}
