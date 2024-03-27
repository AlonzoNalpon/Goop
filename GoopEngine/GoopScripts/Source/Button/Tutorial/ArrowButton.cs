using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button.Tutorial
{
	public class ArrowButton : IButtonClick, IButtonRelease, IButtonHoverEnter, IButtonHoverExit
	{
		protected Random m_rng;
		protected bool m_hovering;
		protected bool m_clicked;

		/*!******************************************************************
		\brief
			Default constructor setting fixed text colour values
		********************************************************************/
		public ArrowButton()
		{
			m_rng = new Random();
		}

		/*!******************************************************************
		\brief
			Callback for when mouse click on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public virtual void OnClick(uint entity)
		{
			m_clicked = true;
			Utils.UpdateSprite(entity, "HTP_Arrow_Left_Disabled");
		}

		/*!******************************************************************
		\brief
			Callback for when mouse release on the entity after click
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public virtual void OnRelease(uint entity)
		{
			m_clicked = false;
			Utils.PlaySoundF("SFX_ButtonClick", (float)m_rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
			if (!m_hovering)
			{
				Utils.UpdateSprite(entity, "HTP_Arrow_Left");
			}
			else
			{
				OnHoverEnter(entity);
			}
		}

		/*!******************************************************************
		\brief
			Callback for when mouse first hovers on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public virtual void OnHoverEnter(uint entity)
		{
      Utils.PlaySoundF("MenuHoverOverSFX2", 0.8f, Utils.ChannelType.SFX, false);
      m_hovering = true;
			Utils.UpdateSprite(entity, "HTP_Arrow_Left_Hover");
		}

		/*!******************************************************************
		\brief
			Callback for when mouse exits hovers on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public virtual void OnHoverExit(uint entity)
		{
			m_hovering = false;
			if (!m_clicked)
			{
				Utils.UpdateSprite(entity, "HTP_Arrow_Left");
			}
		}
	}
}
