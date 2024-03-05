/*!*********************************************************************
\file   MenuQuitGame.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script to go deeper into the menu confirming if the player
wants to "Surrender".

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
  public class MenuQuitGame : IButtonClick, IButtonRelease
  {
    public int Command;
    public int PauseMenuID, DeeperPauseMenuID;
    public void OnClick(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
    }

    public void OnRelease(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base");
			Utils.PlaySoundF("SFX_ButtonClick", 1.0f, Utils.ChannelType.SFX, false);
			if (Command == -1)
			{
				// Return to main menu
				return;
			}
			Utils.DeeperPauseMenu(PauseMenuID, DeeperPauseMenuID);
		}
  }
}
