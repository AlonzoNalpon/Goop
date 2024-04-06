/*!*********************************************************************
\file   MenuHowToPlay.cs
\author loh.j\@digipen.edu
\date   08-February-2024
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

    /*!*********************************************************************
    \brief
      Triggers the How to Play screen from the pause menu
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
    {
      HTP_Popup.m_page = 1;
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), true);
      Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Right"), true);
      base.OnRelease(entity);
			Utils.DeeperPauseMenu(PauseMenuID, DeeperPauseMenuID);
		}
  }
}
