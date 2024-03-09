/*!*********************************************************************
\file   StopGameAudio.cs
\date   04-February-2024
\brief  Script to stop game audio

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Audio
{
	internal class StopGameAudio : IButtonClick
	{
		public void OnClick(uint entity)
		{
			Utils.FadeOutAudio("caveFighting", 1.0f);
			Utils.FadeOutAudio("CaveWithWaterDrops_Loop", 1.0f);
			Utils.FadeOutAudio("Fog", 1.0f);
		}
	}
}
