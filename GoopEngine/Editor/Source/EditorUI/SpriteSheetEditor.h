/*!*********************************************************************
\file   SpriteSheetEditor.h
\author chengen.lau\@digipen.edu
\date   10-March-2024
\brief  Class responsible for ImGui window for editing sprite sheet
        config data (slices, stacks, speed etc.). It uses RTTR to
        display the values loaded into the asset manager and then
        saving it back to the file afterwards to reload.
  
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include <vector>
#include <Serialization/SpriteData.h>

namespace GE::EditorGUI
{

  class SpriteSheetEditor
  {
  public:
    /*!*********************************************************************
    \brief
      Creates the ImGui window for the Sprite Sheet Editor
    \param tabName
      The name of the window
    ************************************************************************/
    static void CreateContent(const char* tabName);

    /*!*********************************************************************
    \brief
      Checks if the sprite sheet editor is currently toggled
    \return
      True if it is toggled and false otherwise
    ************************************************************************/
    static inline bool IsToggled() noexcept { return m_isToggled; }

    /*!*********************************************************************
    \brief
      Toggles the sprite sheet editor on/off
    ************************************************************************/
    static void ToggleSpriteSheetEditor();

  private:
    static bool m_isToggled, m_loadedThisSession;
    static std::vector<Serialization::SpriteData> m_spriteSheetData;
  };

} // namespace GE::EditorGUI
#endif
