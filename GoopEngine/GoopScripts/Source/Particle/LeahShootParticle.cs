/*!*********************************************************************
\file   LeahShootParticle.cs
\author w.chinkitbryan\@digipen.edu
\date   02-March-2024
\brief  Script to trigger particle effects for Leah's attack
				animation

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Particle
{
	internal class LeahShootParticle : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.PlaySingleParticle(Utils.GetEntity("Leah Particle Emitter"));
		}
	}
}
