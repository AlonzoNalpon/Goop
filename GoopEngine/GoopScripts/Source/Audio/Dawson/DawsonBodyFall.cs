﻿/*!*********************************************************************
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
	public class DawsonBodyFall : IAnimationEvent
	{
		public int startRange, endRange;
		public void PlayEvent(uint entity)
		{
      startRange = 0;
      endRange = 0;
      Utils.PlayRandomSound(startRange, endRange, entity, 0.7f);
		}
	}
}