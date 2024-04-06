/*!*********************************************************************
\file   GameNameFlicker.cs
\author w.chinkitbryan\@digipen.edu
\date   14-March-2024
\brief
	This file contains a simple vec4 template struct.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
	internal class GameNameFlicker
	{
		Random m_rng;
		double m_currTime;
		double m_targetTime;

		GameNameFlicker()
		{
			m_rng = new Random();
		}

    /*!******************************************************************
		\brief
			Triggers the blink animation of the title based on a randomized
			time interval
		\param dt
			The delta time of the current frame
		********************************************************************/
    public void OnUpdate(double dt)
		{
			if (m_currTime > m_targetTime)
			{
				m_targetTime = 0.5 + m_rng.NextDouble() * 4.0;
				m_currTime = 0;
				Utils.PlayTransformAnimation(Utils.GetEntity("[Image] Title"), "Blink");
			}
			else
			{
				m_currTime += dt;
			}
		}
	}
}
