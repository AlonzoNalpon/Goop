/*!*********************************************************************
\file   LeahFlashBangs.cs
\date   04-March-2024
\brief  Script to play Leah smokescreen SFX

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
	internal class LeahSmokeScreen : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.PlaySoundF("SFX_Leah_Smokescreen", 1.0f, Utils.ChannelType.SFX, false);
		}
	}
}
