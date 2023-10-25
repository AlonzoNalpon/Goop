/*!*********************************************************************
\file   GivingMyself90MinToFigureThisOut.h
\author chengen.lau\@digipen.edu
\date   25-October-2023
\brief  As of 8:40pm 10/25/23, I think this will take too much time to
        set-up for M2. I will halt this for now and just serialize
        normally.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/Entity/Entity.h>
#include <rttr/type>

namespace GE
{
  namespace ObjectFactory
  {
    rttr::instance GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type);


  } // namespace ObjectFactory
} // namespace GE