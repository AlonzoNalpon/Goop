/*!*********************************************************************
\file   GameOverToMainMenu.cs 
\author w.chinkitbryam\@digipen.edu
\date   06 March 2024
\brief  
  Button behaviour for changing victor/defeat screen back to main menu
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
	internal class GameOverToMainMenu : TextButtonBase
	{
		/*!******************************************************************
		\brief
			Callback for when mouse release on the entity after click
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public override void OnRelease(uint entity)
    {
			base.OnRelease(entity);
			Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "MainMenu");
		}
  }
}
