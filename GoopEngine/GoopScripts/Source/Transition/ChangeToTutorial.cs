/*!*********************************************************************
\file   ChangeToGameScene.cs
\author c.phua\@digipen.edu
\date   08-February-2024
\brief
  Script used to trigger the changing of scenes to Tutorial.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  internal class ChangeToTutorial : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.TransitionToScene("Tutorial");
      Gameplay.Tutorial.ResetTutorial();
    }
  }
}
