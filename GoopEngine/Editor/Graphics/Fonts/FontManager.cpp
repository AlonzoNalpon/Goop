#include <Graphics/Fonts/FontManager.h>
#include <Debugger/Exception/Exception.h>
namespace Graphics::Fonts
{
  FontManager::FontManager()
  {
    FT_Error error{ FT_Init_FreeType(&library) };
    if (error)
    {
      throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Failed to initialize freetype!"));
    }
  }
  bool FontManager::LoadFont(std::string const& name, std::string const& path, GLint fontSize)
  {
    //FT_Face face;
    //if (FT_New_Face(library, path.c_str(), ))
    return false;
  }
}
