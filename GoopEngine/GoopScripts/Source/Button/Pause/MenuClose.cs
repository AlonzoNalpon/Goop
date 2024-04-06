/*!*********************************************************************
\file   MenuClose.cs
\author loh.j\@digipen.edu
\date   08-February-2024
\brief
  Button script used for closing the pause menu.

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
  public class MenuClose : TextButtonBase
  {
    public int PauseMenuID;
    public int DeeperPauseMenuID;
    public MenuClose() { }

    /*!*********************************************************************
    \brief
      Hides the pause menu based on its current state
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
		{
      m_clicked = false;
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
          Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), false);
          Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Left"), false);
          break;
				default:
					break;
			}
		}
	}
}
