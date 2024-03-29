/*!*********************************************************************
\file   ChangeToCutScenes.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
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
      Utils.FadeInAudio("VL_Leah_CutScene_1", 0.75f, 1.0f);
		}
	}
}
