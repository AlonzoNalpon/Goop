/*!*********************************************************************
\file       ComboPage.cs
\author     a.nalpon\@digipen.edu
\date       10-March-2024
\brief      Simple script to display combo list on tab button
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.UI;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Source.Gameplay
{
  public class ComboPage : Entity
  {
    /*!*********************************************************************
    \brief
      Displays the combo page when the TAB key is pressed
    \param deltaTime
      The delta time of the current frame
    ************************************************************************/
    public void OnUpdate(double deltaTime)
    {
      // don't trigger when paused
      if (PauseManager.IsGamePaused())
      { 
        return;
      }

      if (Utils.IsKeyHeld(Input.KeyCode.TAB))
      {
          Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), true);
      }

      if (Utils.IsKeyReleased(Input.KeyCode.TAB))
      {
          Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), false);
      }
    }
  }
}
