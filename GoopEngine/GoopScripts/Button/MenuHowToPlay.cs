/*!*********************************************************************
\file   MenuHowToPlay.cs
\author loh.j@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used to transition from pause menu to how to play screen.

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
  public class MenuHowToPlay : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    public int PauseMenuID, DeeperPauseMenuID;

    public void OnClick(uint entity)
    {
      Utils.DeeperPauseMenu(PauseMenuID, DeeperPauseMenuID);
    }

    public void OnHoverEnter(uint entity)
    {
      // Console.WriteLine(entity.ToString() + "Enter");
    }

    public void OnHoverExit(uint entity)
    {
      // Console.WriteLine(entity.ToString() + "Exit");
    }
  }
}
