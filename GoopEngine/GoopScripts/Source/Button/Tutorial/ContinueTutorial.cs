/*!*********************************************************************
\file   ContinueTutorial.cs
\author c.phua\@digipen.edu
\date   14-March-2024
\brief  Script to continue with the tutorial from the "Skip Tutorial"
        prompt.

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
  public class ContinueTutorial : TextButtonBase
  {
    /*!*********************************************************************
    \brief
      Hides and shows the relevant buttons upon clicking the option in the
      prompt
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
    { 
      base.OnRelease(entity);
      Utils.SetIsActiveEntity(Utils.GetEntity("Skip_Tutorial_Prompt"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity("SkipButton"), true);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{++Tutorial.m_tut}"), true);
    }
  }
}