/*!*********************************************************************
\file   MenuMainMenu.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used to send to main menu screen, while setting the 
pause menu to default state.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
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
  public class MenuMainMenu : IButtonClick, IButtonRelease
  {
    public void OnClick(uint entity)
    {
			StopGameAudio audio = new StopGameAudio();
			audio.OnClick(0u);
      Utils.PlaySoundF("SFX_ButtonClick", 1.0f, Utils.ChannelType.SFX, false);
			Utils.ToMainMenu();
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
    }
    public void OnRelease(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base");
    }

  }
}
