﻿/*!*********************************************************************
\file   ChangeToCutScenes.cs
\author loh.j\@digipen.edu
\date   08-February-2024
\brief
  Script used to trigger the changing of scenes to CutScene.

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
	internal class ChangeToCutScenes : IAnimationEvent
	{
		public void PlayEvent(uint entity)
		{
      Utils.TransitionToScene("Cutscene");
      Utils.FadeOutAudio("menu", 1.0f);
      Utils.FadeOutAudio("CaveWithWaterDrops_Loop", 1.0f);
      Utils.FadeOutAudio("Fog", 1.0f);
    }
	}
}
