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
    static private bool m_lastPaused = false;
    static private bool m_pause = false;
    static private bool m_deeperPause = false; // Inside another menu that is not the basic pause screen

    static PauseManager() { }

    static public bool PauseStateChanged() {
      if (m_lastPaused != m_pause)
      {
        m_lastPaused = m_pause;
      }
      return m_lastPaused; 
    }

    static public bool IsPaused() { return m_pause; }

    static public bool IsDeeperPaused() { return m_deeperPause; }

    static public void SetDeeperPause(bool val) { m_deeperPause = val; }

    static public void SetPause(bool val) { m_pause = val; }

    static public void UnpauseMenu()
    {
      Utils.SetIsActiveEntity(27, false);
      UI.PauseManager.SetPause(false);
    }

  }
}
