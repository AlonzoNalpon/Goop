using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{ 
  public class ContinueTutorial : TextButtonBase
  {
    public override void OnRelease(uint entity)
    { 
      base.OnRelease(entity);
      Utils.SetIsActiveEntity(Utils.GetEntity("Skip_Tutorial_Prompt"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity("SkipButton"), true);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Tutorial_{++Tutorial.m_tut}"), true);
    }
  }
}