﻿/*!*********************************************************************
\file   TutorialBtn.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Script for tutorial's end turn button behaviour. Functions
				similar to the game scene's version.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Button
{
	internal class TutorialBtn : TextButtonBase
	{
		TutorialBtn() : base() { }

		private bool m_disabledEndTurn;

    /*!*********************************************************************
    \brief
      Reverts the button to its default state
    ************************************************************************/
    public void Enable()
		{
			m_disabledEndTurn = false;

			// Have to do this to get entity as caller has
			// no idea what the entity of this is from
			uint entity = Utils.GetEntity("Button_EndTurn");

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
		}

    /*!*********************************************************************
    \brief
      Disables the button by setting the flag
    ************************************************************************/
    public void Disable()
		{
			m_disabledEndTurn = true;
		}

		/*!******************************************************************
		\brief
			Callback for when mouse click on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public override void OnClick(uint entity)
		{
			if (!m_disabledEndTurn)
			{
				base.OnClick(entity);
			}
		}

		/*!*********************************************************************
     \brief
       Triggers when release event is detected
     \param entity
       The id of the current entity
    ************************************************************************/
		public override void OnRelease(uint entity)
		{
			if (!m_disabledEndTurn)
			{
				m_clicked = false;
				Disable();
				Utils.PlaySoundF("SFX_ButtonClick", (float)m_rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
				Gameplay.Tutorial gm = (Gameplay.Tutorial)Utils.GetScript("GameSystem", "Tutorial");
				gm.EndTurn();
			}
		}

		/*!******************************************************************
		\brief
			Callback for when mouse first hovers on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public override void OnHoverEnter(uint entity)
		{
			m_hovering = true;
			if (!m_disabledEndTurn)
			{
				Utils.UpdateSprite(entity, "Button_Base_Hover");
				Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.HIGHLIGHT_TEXT];
				Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
			}
		}

		/*!******************************************************************
		\brief
			Callback for when mouse exits hovers on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public override void OnHoverExit(uint entity)
		{
			m_hovering = false;
			if (!m_disabledEndTurn)
			{
				if (!m_clicked)
				{
					Utils.UpdateSprite(entity, "Button_Base");
					Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT];
					Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
				}
			}
		}
	}
}