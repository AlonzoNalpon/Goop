/*!*********************************************************************
\file   ChangeToHTP.cs
\author loh.j@digipen.edu
\date   08 Febuary 2024
\brief
  Script used to trigger the changing of scenes to HowToPlay.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  internal class ChangeToHTP : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.TransitionToScene("HowToPlay");
    }
  }
}
