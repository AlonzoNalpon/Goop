﻿using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Button
{
  internal class TutorialBtn : IButtonClick
  {

    public TutorialBtn()
    {

    }
    public void OnClick(uint entity)
    {
      Gameplay.Tutorial gm = (Gameplay.Tutorial)Utils.GetScript("GameSystem", "Tutorial");
      gm.EndTurn();
    }
  }

}