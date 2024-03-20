﻿using GoopScripts.Button;
using GoopScripts.Demo;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button
{
  public class TransitionOut2 : TextButtonBase // in the event there is 2 outgoing buttons
  {
    /*!******************************************************************
		\brief
			Callback for when mouse is released after clicking the entity
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      if (HomeBase.GetLevelToLoad() == 0)
      {
        Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "TransitionOut2");
      }
      else
      {
        Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Game");
        CrossFadeToGameBGM temp = new CrossFadeToGameBGM();
        temp.OnClick(0);
        Utils.FadeInAudio("CaveWithWaterDrops_Loop", 0.486f, 1.0f);
        Utils.FadeInAudio("Fog", 0.753f, 1.0f);
      }
      
    }
  }
}
