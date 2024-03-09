using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Button
{
  public class BackTutorial : IButtonClick
  {
    public BackTutorial() { }
    public void OnClick(uint entity)
    {
      int m_curr = (Tutorial.m_tut)--;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{m_curr}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{Tutorial.m_tut}"), true);
    }
  }
}
