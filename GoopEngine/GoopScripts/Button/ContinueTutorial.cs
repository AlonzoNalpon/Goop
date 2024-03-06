using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class ContinueTutorial : IButtonClick
  {
    public void OnClick(uint entity)
    {
      Utils.SetIsActiveEntity(Utils.GetEntity("Skip_Tutorial_Prompt"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity("SkipButton"), true);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{Tutorial.m_tut}"), true);
    }
  }
}