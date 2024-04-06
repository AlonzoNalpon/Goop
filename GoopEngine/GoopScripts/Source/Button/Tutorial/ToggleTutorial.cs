/*!*********************************************************************
\file   ToggleTutorial.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Script for arrow button to trigger the next step in tutorial.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;
using GoopScripts.Source.Button.Tutorial;

namespace GoopScripts.Button
{
  public class ToggleTutorial : ArrowButton
  {
    /*!*********************************************************************
    \brief
      Increments the tutorial's step and displays the next page
    ************************************************************************/
    public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			int m_prev = Tutorial.m_tut++;

			Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_prev}"), false);
			Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{Tutorial.m_tut}"), true);
		}
	}
}
