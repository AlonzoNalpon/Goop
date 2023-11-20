#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H
#include <ECS/System/System.h>
#include <iostream>

// The involved components:
#include <Component/Sprite.h>			// sprite (ALL MUST HAVE THIS)
#include <Component/SpriteAnim.h> // Moved to SpriteAnimSystem
#include <Component/Transform.h>  
namespace GE::Systems 
{
   /*!
   * \class CameraSystem
   * \brief
   * System that updates camera in scene
   */
  class CameraSystem : public GE::ECS::System
  {
  public:
    /*!*********************************************************************
    \brief
      Renders all entities with model and sprite component.
    \params
    \return
    ************************************************************************/
    void Update();

    void OnDestroyed();
  };
}

#endif
