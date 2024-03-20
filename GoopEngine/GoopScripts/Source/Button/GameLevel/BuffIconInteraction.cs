/*!*********************************************************************
\file   BuffIconInteraction.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  Script for hover effect for buff icons above the healthbar.
        Displays text on the buff info by setting it to active/inactive.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class BuffIconInteraction : IButtonHoverEnter, IButtonHoverExit
  {
    BuffIconInteraction() { }
    public void OnHoverEnter(uint entity)
    {
      Utils.SetBuffIconTextActive(entity, true);
    }

    public void OnHoverExit(uint entity)
    {
      Utils.SetBuffIconTextActive(entity, false);
    }
  }
}
