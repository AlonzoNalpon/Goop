/*!*********************************************************************
\file   HTP_Back_Popup.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Inherits from the ArrowButton class to implement the back button
        for the pause menu's "How To Play" screen.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using GoopScripts.Source.Button.Tutorial;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class HTP_Back_Popup : ArrowButton
  {
    /*!*********************************************************************
    \brief
      Decrements the page number and displays the previous page
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			if (HTP_Popup.m_page == 1)
      {
        return;
      }

      int m_prev = (HTP_Popup.m_page)--;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), true);
    }
  }
}
