/*!*********************************************************************
\file     ToolTip.cs
\author   loh.j\@digipen.edu
\date     25-March-2024
\briefq   Script to display the tool tip in the deck viewer screen

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
  public class Tooltip : IButtonHoverEnter, IButtonHoverExit
  {
    static readonly int LEVELS = 6;
    public int[] tooltipID = new int[LEVELS];
    int m_levelToLoad;

    Tooltip() { }

    /*!******************************************************************
		\brief
			Sets the tooltip entity to active when hovered on
		\param entity
      The entity ID of the current object
		********************************************************************/
    public void OnHoverEnter(uint entity)
    {
      m_levelToLoad = HomeBase.m_levelToLoad;
      if (m_levelToLoad >= LEVELS)
        m_levelToLoad = LEVELS - 1;
      Utils.SetIsActiveEntity((uint)tooltipID[m_levelToLoad], true);
    }

    /*!******************************************************************
		\brief
			Sets the tooltip entity to inactive when unhovered
		\param entity
      The entity ID of the current object
		********************************************************************/
    public void OnHoverExit(uint entity)
    {
      Utils.SetIsActiveEntity((uint)tooltipID[m_levelToLoad], false);
    }
  }
}
