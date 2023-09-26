#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>   // freetype
#include FT_FREETYPE_H  // this macro includes the main freetyp2 header file

#include <AssetManager/AssetManager.h>
#include <map>
#include <string>
#include <def.h>
#include <Graphics/Fonts/Character.h>
namespace Graphics::Fonts
{
  /*!
   * \class FontManager
   * \brief  
   * Manager responsible for creation, deletion and 
   * storage of font data in the GPU.
   */
  class FontManager 
  {
    using FontMap         = std::map<uChar, Character>;
    using FontData        = std::pair<std::string, FontMap>;
    using FontsContainer  = std::vector<FontData>;
  public:
    FontManager();
    bool LoadFont(std::string const& name, std::string const& path, GLint fontSize);
  private:
    FT_Library library;
    FontsContainer m_fonts; // !< all loaded fonts' glyphs stored here
  };
}

#endif
