/*!*********************************************************************
\file   CSharpStructs.h
\author chengen.lau\@digipen.edu
\date   19-January-2024
\brief  This file contains the structs used to display and modify C#
        objects on the inspector.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <vector>
#include <Component/Card.h>
#include <ScriptEngine/ScriptInstance.h>

namespace GE::MONO
{
  struct CharacterAnims
  {
    //std::string m_idle, m_flinch, m_death, m_deathShield;
    ScriptInstance m_characterAnimsInst;
  };
}