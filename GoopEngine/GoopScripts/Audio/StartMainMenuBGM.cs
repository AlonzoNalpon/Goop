/*!*********************************************************************
\file   StartMainMenuBGM.cs
\date   04-February-2024
\brief  Script to start main menu music

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Audio
{
	internal class StartMainMenuBGM
	{
		public void OnCreate()
		{
			Utils.FadeInAudio("menu", 0.9f, 1.0f);
		}
	}
}
