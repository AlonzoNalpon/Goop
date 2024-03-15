/*!*********************************************************************
\file   TextRenderSystem.h
\author a.nalpon\@digipen.edu
\date   3-November-2023
\brief  This file contains the definition for the text rendering system
        . Part of ECS
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef TEXT_RENDER_SYSTEM_H
#define TEXT_RENDER_SYSTEM_H
#include <ECS/System/System.h>
#include <Component/Text.h>
namespace GE::Systems {
  class TextRenderSystem : public GE::ECS::System
  {
  public:
    virtual void LateUpdate();
  };
}

#endif
