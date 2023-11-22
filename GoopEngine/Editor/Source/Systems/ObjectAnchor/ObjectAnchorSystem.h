/*!*********************************************************************
\file   ObjectAnchorSystem.h
\author w.chinkitbryan\@digipen.edu
\date   21-November-2023
\brief  
  System to allow object's transform to anchor to.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <ECS/System/System.h>

namespace GE::Systems
{
  class ObjectAnchorSystem : public ECS::System
  {
    void FixedUpdate();
  };
}
