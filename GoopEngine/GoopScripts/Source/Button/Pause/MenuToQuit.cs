/*!*********************************************************************
\file   MenuToQuit.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Script to attach to button to verify quitting.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button.Pause
{

  public class MenuToQuit : TextButtonBase
  {
		public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      Utils.SetIsActiveEntity(Utils.GetEntity("Confirm Quit"), true);
    }
  }
}
