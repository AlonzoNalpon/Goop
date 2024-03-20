/*!*********************************************************************
\file     ComboTextUpdate.cs
\author   chengen.lau\@digipen.edu
\date     09-February-2024
\brief    Script to clear the combo text when a combo is triggered
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  public class ComboTextUpdate : Entity
  {
    static readonly double TIME_ON_SCREEN = 2.5;
    double m_timer = 0.0;
    bool m_active;

    public ComboTextUpdate(uint entity) : base(entity) { }

    /*!*********************************************************************
    \brief
      Checks if active flag has been set. If set, clears the text
      component after TIME_ON_SCREEN seconds.
    \params deltaTime
      The delta time of the current frame
    ************************************************************************/
    public void OnUpdate(double deltaTime)
    {
      if (!m_active) { return; }

      m_timer += deltaTime;
      if (m_timer >= TIME_ON_SCREEN)
      {
        Utils.SetTextComponent((int)entityID, "");
        m_active = false;
        m_timer = 0.0;
      }
    }

    /*!*********************************************************************
    \brief
      Sets the active flag to true.
    ************************************************************************/
    public void SetActive()
    {
      m_active = true;
    }
  }
}
