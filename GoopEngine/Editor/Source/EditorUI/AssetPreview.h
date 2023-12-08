/*!*********************************************************************
\file   AssetPreview.h
\author loh.j\@digipen.edu
\date   26-November-2023
\brief
  Allows user to view assets from the asset browser.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include "ImGuiUI.h"
#include <ImGui/imgui.h>
#include <AssetManager/AssetManager.h>
namespace GE
{
  namespace EditorGUI
  {
    class AssetPreview
    {
    public:
    /*!******************************************************************
    \brief
      Wrapper to call ImTerm console
    ********************************************************************/
      static void CreateContent();
    };
  }
}
#endif