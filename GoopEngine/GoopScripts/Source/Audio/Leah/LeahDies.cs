/*!*********************************************************************
\file   LeahDies.cs
\date   04-February-2024
\brief  Script to play Leah death SFX

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
	public class LeahDies : IAnimationEvent
	{
		public int startRange, endRange;
		Random rng = new Random();
		public void PlayEvent(uint entity)
		{
			Utils.PlayRandomSound(startRange, endRange, entity, 0.7f + (float)rng.NextDouble() * 0.3f);
		}
	}
}
