/*!*********************************************************************
\file   PauseManager.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Script is used to detect if the game should display the pause
information or not.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.UI
{
  static public class PauseManager
  {
    static int m_pauseState = 0; // 0 - Unpaused, 1 - Paused, 2 - DeeperPaused
    static int m_lastPauseState = 0;

    static PauseManager() { }

    static public bool PauseStateChanged()
    {      
      if (m_lastPauseState != m_pauseState)
      {
        m_lastPauseState = m_pauseState;
        return true;
      }
      return false;
    }

    static public int GetPauseState() { return m_pauseState; }

    static public void SetPauseState(int id) 
    {
      m_pauseState = id; 
      if (m_pauseState != 0)
			{
				Utils.SetTimeScale(0.0f);
                //Utils.PauseChannel(Utils.ChannelType.SFX, true);
                //Utils.PauseChannel(Utils.ChannelType.BGM, true);
      }
      else
			{
				Utils.SetTimeScale(1.0f);
                //Utils.PauseChannel(Utils.ChannelType.SFX, false);
                //Utils.PauseChannel(Utils.ChannelType.BGM, false);
      }
    }
  }
}
