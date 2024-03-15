/*!*********************************************************************
\file   ChangeToDefeat.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Change scene to defeat.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
	internal class ChangeToDefeat : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
			Utils.TransitionToScene("Defeat");
		}
	}
}
