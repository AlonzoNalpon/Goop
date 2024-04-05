/*!*********************************************************************
\file   TutorialTransition.cs 
\author w.chinkitbryan\@digipen.edu
\date   14 March 2024
\brief  Triggers the transition animation for tutorial
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GoopScripts.Button
{
	public class TutorialTransition : TextButtonBase
	{
		public string animToTrigger;
		/*!******************************************************************
		\brief
			Callback for when mouse is released after clicking the entity
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
		public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
      Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), animToTrigger);
		}
	}
}
