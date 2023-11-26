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
