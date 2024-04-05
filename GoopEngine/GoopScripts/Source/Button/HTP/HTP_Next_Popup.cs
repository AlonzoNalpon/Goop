/*!*********************************************************************
\file   HTP_Next_Popup.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Inherits from the ArrowButton class to implement the next button
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
  internal class HTP_Next_Popup : ArrowButton
	{
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      if (HTP_Popup.m_page == 11)
      {
        return;
      }

      int m_prev = (HTP_Popup.m_page)++;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP_Popup.m_page}"), true);
    }
  }
}
