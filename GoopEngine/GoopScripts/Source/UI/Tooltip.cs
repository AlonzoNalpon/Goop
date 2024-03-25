using GoopScripts.Button;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
  public class Tooltip : IButtonHoverEnter, IButtonHoverExit
  {
    static readonly int LEVELS = 6;
    public int[] tooltipID = new int[LEVELS];
    int m_levelToLoad;

    Tooltip() { }

    public void OnHoverEnter(uint entity)
    {
      m_levelToLoad = HomeBase.m_levelToLoad;
      if (m_levelToLoad >= LEVELS)
        m_levelToLoad = LEVELS - 1;
      Utils.SetIsActiveEntity((uint)tooltipID[m_levelToLoad], true);
    }

    public void OnHoverExit(uint entity)
    {
      Utils.SetIsActiveEntity((uint)tooltipID[m_levelToLoad], false);
    }
  }
}
