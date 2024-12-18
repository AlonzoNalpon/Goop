﻿/*!*********************************************************************
\file   HTP_Next.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Inherits from the ArrowButton class to implement the next button
        for the main menu's "How To Play" screen.

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
  public class HTP_Next : ArrowButton
	{
    /*!*********************************************************************
    \brief
      Increments the page number and displays the next page
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      if (HTP.m_page == 11)
      {
        return;
      }

      int m_prev = (HTP.m_page)++;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP.m_page}"), true);
    }
  }
}
