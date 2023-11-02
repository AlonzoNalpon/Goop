/*!*********************************************************************
\file   FontManager.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of the font manager class
      It's still under construction 
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>   // freetype
#include FT_FREETYPE_H  // this macro includes the main freetyp2 header file

#include <AssetManager/AssetManager.h>
#include <map>
#include <string>
#include <def.h>
#include <Graphics/Fonts/Character.h>
#include <Graphics/Model.h>
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
  public:
    using FontMap         = std::map<uChar, Character>;
    using FontMapFontMap  = std::map<Graphics::gObjID ,FontMap>;
    using FontID_LT       = std::map<std::string, Graphics::gObjID>;
    using FontName_LT     = std::map<Graphics::gObjID, std::string>;
    void Init(GLuint fontShader, GLuint fontQuad);
    void LoadFont(std::string const& name, std::string const& path, GLint fontSize = 256);
  private:
    FT_Library        m_library;
    FontMapFontMap    m_fonts;      // !< all loaded fonts' glyphs stored here
    FontID_LT         m_fontIDLT;     // !< lookup table to get font ID from string
    FontName_LT       m_fontNameLT; // !< lookup table to get name from ID
    GLuint            m_fontQuad;
    GLuint            m_fontShader;

  public:
    FontMap const&    GetFontMap(Graphics::gObjID fontID)const;
    std::string       GetFontName(Graphics::gObjID fontName)const;
    Graphics::gObjID  GetFontID(std::string const& name)const;
    GLuint const&     fontModel{ m_fontQuad };
    GLuint const&     fontShader{ m_fontShader };
  };
}

#endif
