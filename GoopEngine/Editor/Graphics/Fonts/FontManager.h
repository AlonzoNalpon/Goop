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
<<<<<<< Updated upstream
    using FontData        = std::pair<std::string, FontMap>;
    using FontsContainer  = std::vector<FontData>;
  public:
    FontManager();
    bool LoadFont(std::string const& name, std::string const& path, GLint fontSize);
=======
    using FontsContainer  = std::vector<FontMap>;
    using FontsLT         = std::map<std::string, gObjID>;
  public:
    void Init();
    gObjID LoadFont(std::string const& name, std::string const& path, GLint fontSize);
    gObjID GetFontID(std::string const& name);
    Character const& GetCharacter(gObjID font, uChar ch);
>>>>>>> Stashed changes
  private:
    FT_Library library;
    FontsContainer m_fonts; // !< all loaded fonts' glyphs stored here
    FontsLT m_fontsLT; // !< all loaded fonts' glyphs stored here
  };
}

#endif
