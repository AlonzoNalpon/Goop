#pragma once
#ifndef NO_IMGUI
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
      static void CreateContent();
    };
  }
}
#endif