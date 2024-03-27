/*!*********************************************************************
\file   MenuClose.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used for closing the pause menu.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
      m_clicked = false;
      Utils.PlaySoundF("Menu-Quit", (float)m_rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
      if (!m_hovering)
      {
        Utils.UpdateSprite(entity, "Button_Base");
        Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT];
        Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
      }
      else
      {
        OnHoverEnter(entity);
      }
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
