using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class ToggleTutorial : IButtonClick
  {
    int m_tut;

    ToggleTutorial()
    {
      m_tut = 1;
    }
    public void OnClick(uint entity)
    {
      int m_prev = m_tut++;

      //Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_prev}"), false);
      //Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_tut}"), true);
    }

    public int GetTutorial()
    {
      return m_tut;
    }

    public int IncTutorial()
    {
      return ++m_tut;
    }
  }
}
