/*!*********************************************************************
\file   MenuQuitGame.cs
\author loh.j@digipen.edu
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
  public class MenuQuitGame : IButtonClick
  {
    public int Command;
    public int PauseMenuID, DeeperPauseMenuID;
    public void OnClick(uint entity)
    {
      if (Command == -1)
      {
        // Return to main menu
        return;
      }
      Utils.DeeperPauseMenu(PauseMenuID,DeeperPauseMenuID);
    }
  }
}
