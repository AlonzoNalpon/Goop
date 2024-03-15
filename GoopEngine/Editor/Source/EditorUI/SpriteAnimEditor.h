/*!*********************************************************************
\file   SpriteAnimEditor.h
\author a.nalpon\@digipen.edu
\date   15-March-2024
\brief  Displays the ImGui window for the sprite animation editor.
        Allows adding of events at certain keyframes of animations.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIM_EDITOR
#define SPRITE_ANIM_EDITOR
#ifndef IMGUI_DISABLE
#include <Graphics/Def/GraphicsTypes.h>
namespace GE::EditorGUI
{
  class SpriteAnimEditor
  {
    private:
      static Graphics::gObjID currAnimation; //!< currently selected in editor
    public:
    static void CreateContent();
  };
}
#endif
#endif
