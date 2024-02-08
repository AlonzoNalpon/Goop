/*!*********************************************************************
\file   LeahDies.cs
\date   04-February-2024
\brief  Script to play Leah death SFX

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Gameplay
{
	internal class LeahDies : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
		}
	}
}
