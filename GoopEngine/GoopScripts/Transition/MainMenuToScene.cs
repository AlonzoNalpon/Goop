﻿/*!*********************************************************************
\file   MainMenuToScene.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Script used to trigger specific animation of the TransitionOut 
entity to trigger the specific script attached to it after the animation
ends.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Transition
{
  public class MainMenuToScene : IButtonClick, IButtonRelease
  {
    public int Scene;
    public void OnClick(uint entity)
    {
      Utils.PlaySoundF("SFX_ButtonClick", 1.0f, Utils.ChannelType.SFX, false);
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
      switch (Scene)
      {
        case 0:
					Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Cutscene");
					break;
        case 1: // LOAD GAME

          break;
        case 2:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Options");
          break;
        case 3:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "HowToPlay");
          break;
        case 4:
          Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Credits");
          break;
        default: break;
      }
    }

    public void OnRelease(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base");
    }
  }
}
