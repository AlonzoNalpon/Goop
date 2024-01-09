/*!*********************************************************************
\file   AssetTypes.h
\date   6-January-2024
\brief  Contains the enum to represent the file types handled by the
        asset manager.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

namespace GE::Assets
{
  enum AssetType
  {
    NONE,
    IMAGES,
    CONFIG,
    ANIMATION,
    AUDIO,
    SCENE,
    PREFAB,
    SHADERS,
    FONTS
  };
}
