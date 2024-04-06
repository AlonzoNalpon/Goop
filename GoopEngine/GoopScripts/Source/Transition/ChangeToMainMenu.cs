/*!*********************************************************************
\file   ChangeToMainMenu.cs 
\author w.chinkitbryan\@digipen.edu
\date   12-February-2024
\brief  
  Animation behaviour to switch to main menu
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;

namespace GoopScripts.Transition
{
	internal class ChangeToMainMenu : IAnimationEvent
	{
		/*!******************************************************************
		\brief
			Callback for when a frame of an animation occurs
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
		public void PlayEvent(uint entity)
		{
			Utils.TransitionToScene("MainMenu");
		}
	}
}
