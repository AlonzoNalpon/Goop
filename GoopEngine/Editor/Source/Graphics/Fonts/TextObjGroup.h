#ifndef TEXT_OBJ_GROUP_H
#define TEXT_OBJ_GROUP_H
#include <Graphics/Fonts/TextObjInfo.h>
namespace GE::Graphics::Fonts
{
  struct TextObjGroup
  {
    std::vector<TextObjInfo> textObjects;
    // Other parameters like spacing can be added here ...
    struct
    {
      bool dirty : 1 = false; //!< should the information be updated?
      // a dirty concat?
    }flags;
  };
}
#endif
