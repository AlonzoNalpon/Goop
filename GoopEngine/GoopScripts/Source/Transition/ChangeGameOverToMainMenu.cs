/*!*********************************************************************
\file   ChangeGameOverToMainMenu.cs 
\author w.chinkitbryan\@digipen.edu
\date   12 February 2024
\brief  
  Animation behaviour to switch the game over screen to main menu
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Audio;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
	internal class ChangeGameOverToMainMenu : IAnimationEvent
	{
		/*!******************************************************************
		\brief
			Callback for when a frame of an animation occurs
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
		public void PlayEvent(uint entity)
		{
			// Hack here and just invoke the stop game audio onclick event
			StopGameAudio temp = new StopGameAudio();
			temp.OnClick(entity);
			Utils.TransitionToScene("MainMenu");
		}
	}
}
