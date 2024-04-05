/*!*********************************************************************
\file   DigipenSplashScreen.cs 
\author w.chinkitbryan\@digipen.edu
\date   09 March 2024
\brief  
  Digipen Splash Screen that plays upon launching the game
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button
{
	public class DigipenSplashScreen : IButtonClick, IAnimationEvent
	{
		/*!******************************************************************
		\brief
			Callback for when mouse click on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public void OnClick(uint entity)
		{
			Utils.TransitionToScene("FMODSplashScreen");
		}

		/*!******************************************************************
		\brief
			Callback for when an animation
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public void PlayEvent(uint entity)
		{
			OnClick(entity);
		}
	}
}
