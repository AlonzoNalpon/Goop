/*!*********************************************************************
\file   FPS.cs
\author a.nalpon\@digipen.edu
\date   09-February-2024
\brief
  Script for updating an FPS counter text object

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.UI
{
  public class FPS : Entity
	{
		static bool shouldShow;
		uint fpsEntity;

		public FPS()
		{
			shouldShow = false;
			fpsEntity = Utils.GetEntity("FPSCounter");
			Utils.SetTextColor(fpsEntity, 0, 0, 0, 0);
		}

    /*!******************************************************************
		\brief
			Updates the FPS entity with the current game's FPS
		\param deltaTime
			The delta time of the current frame
		********************************************************************/
    public void OnUpdate(double deltaTime)
    {
			if (Utils.IsKeyTriggered(Input.KeyCode.P))
      {
        shouldShow = !shouldShow;
        if (!shouldShow)
        {
					Utils.SetTextColor(fpsEntity, 0, 0, 0, 0);
				}
        else
				{
					Utils.SetTextColor(fpsEntity, 240, 255, 0, 255);
				}
			}

			// SET FPS TEXT TO FPS
			double fps = Utils.GetFPS();
			string text = string.Format("{0:N2}", fps);
			Utils.SetTextComponent((int)fpsEntity, text);
    }
  }
}
