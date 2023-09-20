/*!*********************************************************************
\file   RendererComponent.h
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  A rendering component that holds opaque pointers to 
texture and model data. This dataset is used to provide information
for render calls to the graphics engine
\note the position should be removed at some point (should be another component)
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H
#include <ECS/Component/Component.h>
#include <Graphics/Def/GraphicsTypes.h>
namespace GE::Components 
{
  struct Renderer : public GE::ECS::Component
  {
    GLuint model;             //!< handle to a model that includes shader (for graphics engine)
    // can easily add more for special effects
  };

}
#endif
