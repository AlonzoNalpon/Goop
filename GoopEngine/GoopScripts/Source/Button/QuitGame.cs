/*!*********************************************************************
\file   QuitGame.cs 
\author w.chinkitbryam\@digipen.edu
\date   06 March 2024
\brief  
  Quit game button behaviour
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class QuitGame : TextButtonBase
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
      Utils.DispatchQuitEvent();
    }
  }
}
