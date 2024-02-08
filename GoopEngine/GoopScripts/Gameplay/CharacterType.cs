/*!*********************************************************************
\file   CharacterTypes.cs
\author chengen.lau\@digipen.edu
\date   10-January-2024
\brief  
Types of characters in game.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  public enum CharacterType
  {
    PLAYER,
    BASIC_ENEMY,
    BOSS_P1,  // phase 1
    BOSS_P2,  // phase 2
  }
}
