/*!*********************************************************************
\file   SurrenderToMainMenu.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Script to attach to button to go back to main menu from game.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.UI;

namespace GoopScripts.Button
{
	internal class SurrenderToMainMenu : TextButtonBase
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
