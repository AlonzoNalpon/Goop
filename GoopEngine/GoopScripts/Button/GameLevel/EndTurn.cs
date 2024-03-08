/*!*********************************************************************
\file   ReturnFromQueue.cs
\author chengen.lau\@digipen.edu
\date   27-January-2024
\brief  Script for button events related to cards in the queue. Moves
        the selected card back to hand on click and triggers on hover
        events.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static GoopScripts.Button.TextButtonBase;

namespace GoopScripts.Button
{
  public class EndTurn : TextButtonBase
  {
    public int disabledEndTurn;
    /*!*********************************************************************
     \brief
       Triggers when release event is detected
     \param entity
       The id of the current entity
    ************************************************************************/
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      Gameplay.GameManager gm = (Gameplay.GameManager)Utils.GetScript("GameSystem", "GameManager");
      gm.EndTurn();
    }
  }
}