/*!*********************************************************************
\file   MenuResume.cs
\author loh.j\@digipen.edu
\date   08 Febuary 2024
\brief
  Button script used to hide the pause menu.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuResume : IButtonClick
  {
    public int PauseMenuID;

    public void OnClick(uint entity)
    {
      Utils.PlaySoundF("SFX_ButtonClick", 1.0f, Utils.ChannelType.SFX, false);
      Utils.UnpauseMenu(PauseMenuID);
    }
  }
}
