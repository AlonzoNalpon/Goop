/*!*********************************************************************
\file   MenuClose.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used for closing the pause menu.

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
  public class MenuClose : TextButtonBase
  {
    public int PauseMenuID;
    public int DeeperPauseMenuID;
    public MenuClose() { }

		public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			switch (UI.PauseManager.GetPauseState())
			{
				case 0:
					Utils.PauseMenu(PauseMenuID);
					break;
				case 1:
					Utils.UnpauseMenu(PauseMenuID);
					break;
				case 2:
					Utils.UndeeperPause(PauseMenuID, DeeperPauseMenuID);
					break;
				default:
					break;
			}
		}
	}
}
