/*!*********************************************************************
\file   RenderingSystem.h
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the RenderingSystem class.
FOR ECS.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H
#include <ECS/System/System.h>
#include <iostream>

// The involved components:
#include <Component/Model.h>			// models (ALL MUST HAVE THIS)
#include <Component/Sprite.h>			// sprite (ALL MUST HAVE THIS)
#include <Component/SpriteAnim.h> // Moved to SpriteAnimSystem
#include <Component/Transform.h>  
namespace GE::Systems {
  /*!
   * \class RenderSystem
   * \brief  
   * System that renders all objects with models and sprites.
   */
  class RenderSystem : public GE::ECS::System
  {
	public:
    /*!*********************************************************************
    \brief
      Renders all entities with model and sprite component.
    \return
    ************************************************************************/
		void LateUpdate();
  };
}

#endif
