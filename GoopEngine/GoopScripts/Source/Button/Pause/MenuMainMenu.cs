/*!*********************************************************************
\file   MenuMainMenu.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used to send to main menu screen, while setting the 
pause menu to default state.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Audio;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuMainMenu : TextButtonBase
  {
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
			StopGameAudio audio = new StopGameAudio();
			audio.OnClick(0u);
			Utils.ToMainMenu();
		}

  }
}
