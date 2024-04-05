/*!*********************************************************************
\file   CrossFadeToBGM.cs
\date   15-March-2024
\brief  Script to cross fade to BGM

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
using GoopScripts.Mono;

namespace GoopScripts.Demo
{
	internal class CrossFadeToBGM : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.FadeOutAudio("caveFighting", 0.8f);
			Utils.FadeOutAudio("CaveWithWaterDrops_Loop", 0.8f);
			Utils.FadeOutAudio("Fog", 0.8f);
			Utils.FadeInAudio("menu", 0.8f, 1.0f);
		}
	}
}
