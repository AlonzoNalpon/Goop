using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Button
{
  public class ToggleTutorial : IButtonClick
  {
    public ToggleTutorial() { }
    public void OnClick(uint entity)
    {
      int m_prev = (Tutorial.m_tut)++;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{Tutorial.m_tut}"), true);
    }
  }
}
