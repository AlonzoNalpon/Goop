/*!*********************************************************************
\file   MenuHowToPlay.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used to transition from pause menu to how to play screen.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuHowToPlay : TextButtonBase
  {
    public int PauseMenuID, DeeperPauseMenuID;

    public override void OnRelease(uint entity)
    {
      HTP_Popup.m_page = 1;
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), true);
      base.OnRelease(entity);
			Utils.DeeperPauseMenu(PauseMenuID, DeeperPauseMenuID);
		}
  }
}
