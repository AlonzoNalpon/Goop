using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Particle
{
	internal class DawsonAttackParticle : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.PlaySingleParticle(Utils.GetEntity("Dawson Particle Emitter 2"));
		}
	}
}
