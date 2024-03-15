/*!*********************************************************************
\file   TransitionOut.cs 
\author w.chinkitbryan\@digipen.edu
\date   09 March 2024
\brief  
	Triggers the transition animation
 
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
	public class TransitionOut : TextButtonBase
	{
		/*!******************************************************************
		\brief
			Callback for when mouse is released after clicking the entity
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
		public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);			
			Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "TransitionOut");
		}
	}
}
