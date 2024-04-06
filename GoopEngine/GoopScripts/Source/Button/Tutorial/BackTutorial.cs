/*!*********************************************************************
\file   BackTutorial.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Script for arrow button to return to the previous step in
        tutorial.

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
  public class BackTutorial : ArrowButton
  {
    /*!*********************************************************************
    \brief
      Decrements the tutorial's step and displays the previous page
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			int m_curr = (Tutorial.m_tut)--;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_curr}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{Tutorial.m_tut}"), true);
    }
  }
}
