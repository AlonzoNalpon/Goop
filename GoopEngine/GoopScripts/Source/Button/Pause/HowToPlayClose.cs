/*!*********************************************************************
\file   HowToPlayClose.cs
\author w.chinkitbryan\@digipen.edu
\date   09 March 2024
\brief
  Button script used for closing the how to play menu.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button.Pause
{
	public class HowToPlayClose : IButtonRelease
	{
		public int PauseMenuID;
		public int DeeperPauseMenuID;
		public HowToPlayClose() { }

		public void OnRelease(uint entity)
		{
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
