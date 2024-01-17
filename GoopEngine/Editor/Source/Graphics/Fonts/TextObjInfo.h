#ifndef TEXT_OBJ_INFO_H
#define TEXT_OBJ_INFO_H
#include <Graphics/Def/GraphicsTypes.h>
#include <utility>
namespace GE::Graphics::Fonts
{
  struct TextObjInfo
  {
    gVec2 position;
    std::pair<size_t, size_t> substr;
  };
};

#endif
