﻿/*!*********************************************************************
\file   ChangeToGameScene.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Script used to trigger the changing of scenes to Game.

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
  internal class ChangeToGameScene : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.TransitionToScene("Game");
      Gameplay.Tutorial.m_tutorialOn = false;
      Gameplay.GameManager.ResetGameManager();
    }
  }
}
