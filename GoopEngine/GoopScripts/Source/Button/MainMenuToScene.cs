/*!*********************************************************************
\file   MainMenuToScene.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Script used to trigger specific animation of the TransitionOut 
entity to trigger the specific script attached to it after the animation
ends.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Demo;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using GoopScripts.Serialization;

namespace GoopScripts.Transition
{
  public class MainMenuToScene : TextButtonBase
  {
		public string test;
    public int Scene;
    private bool m_shouldTransition;
    public double m_delay;
    private double m_currentTime;

		public MainMenuToScene()
		{
			m_currentTime = 0.0f;
			m_shouldTransition = false;
		}

    
    public override void OnRelease(uint entity)
    {      
			base.OnRelease(entity);
			Utils.PlayTransformAnimation(Utils.GetEntity("[Image] Button Panel"), "GoUp");
      m_shouldTransition = true;
			m_currentTime = 0.0f;
    }

		public void OnUpdate(double dt)
		{
      if (m_shouldTransition)
			{
				if (m_currentTime > m_delay)
				{
					switch (Scene)
					{
						case 0:
							SerialReader.GenerateDefaultSave();
              Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Cutscene");
							break;
						case 1: // LOAD GAME
							Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "Game");
							CrossFadeToGameBGM temp = new CrossFadeToGameBGM();
							temp.OnClick(0);
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
					m_shouldTransition = false;
					m_currentTime = 0.0f;
				}
				else
				{
					m_currentTime += dt;
				}
			}
    }
  }
}
