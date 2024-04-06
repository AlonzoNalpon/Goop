/*!*********************************************************************
\file   MainMenuNoQuit.cs
\author loh.j\@digipen.edu
\date   06-March-2024
\brief  
  Script to attach to button to move scene
 
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
	public class MainMenuNoQuit : TextButtonBase
	{
    /*!*********************************************************************
    \brief
      Hides the quit confirmation upon clicking of the button
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			Utils.SetIsActiveEntity(Utils.GetEntity("Confirm Quit"), false);
		}
	}
}
