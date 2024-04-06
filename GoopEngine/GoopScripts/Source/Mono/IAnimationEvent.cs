/*!*********************************************************************
\file   IAnimationEvent.cs
\date   04-February-2024
\brief  Definition of IAnimationEvent interface, which is called when
				an animation event is triggered.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
	interface IAnimationEvent
	{
		void PlayEvent(uint entity);
	}
}
