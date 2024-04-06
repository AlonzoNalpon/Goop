/*!*********************************************************************
\file   FontManager.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of the font manager class
      It's still under construction 
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
namespace GE::Graphics::Fonts
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
    enum FontAlign : unsigned int
    {
      LEFT = 0u,
      CENTER,
      RIGHT
    };
    struct FontLineInfo
    {
      size_t	idx; // index of character that's in a new line
      GLfloat baseOffset; // 0 for left, -width/2 for center, -width for right
    };
    /*!*********************************************************************
    \brief
      Initializes the font manager by initializing freetype.
    \params
      fontShader  the shader of the font objects to render with
      fontQuad    the model of the font quad to render with
    \return
    ************************************************************************/
    void Init(GLuint fontShader, GLuint fontQuad);

    /*!*********************************************************************
    \brief
      Loads a font into the manager.
    \params
      name name of font (for obtaining the font from other systems)
      path path to the font
      fontSize size of the font (higher = better resolutions)
    \return
    ************************************************************************/
    void LoadFont(std::string const& name, std::string const& path, GLint fontSize = 256);
  private:
    using FontMap         = std::map<uChar, Character>;
    using FontMapFontMap  = std::map<Graphics::gObjID ,FontMap>;
    using FontID_LT       = std::map<std::string, Graphics::gObjID>;
    using FontName_LT     = std::map<Graphics::gObjID, std::string>;
    using FontScales      = std::map<Graphics::gObjID, GLfloat>;
    using FontHeight      = std::map<Graphics::gObjID, GLfloat>;
    FT_Library        m_library;
    FontMapFontMap    m_fonts;      // !< all loaded fonts' glyphs stored here
    FontID_LT         m_fontIDLT;     // !< lookup table to get font ID from string
    FontName_LT       m_fontNameLT; // !< lookup table to get name from ID
    GLuint            m_fontQuad;
    GLuint            m_fontShader;
    FontScales        m_fontScales;
    FontHeight        m_fontHeight;
  public:
    /*!*********************************************************************
    \brief
      Obtain the read-only reference to font map corresponding to ID.
    \params
      fontID ID of font
    \return
      
    ************************************************************************/
    FontMap const&    GetFontMap(Graphics::gObjID fontID)const;

    /*!*********************************************************************
    \brief
      Get the name of a font from a supplied ID.
    \params
      fontID ID of font
    \return
      the name of font
    ************************************************************************/
    std::string       GetFontName(Graphics::gObjID fontID)const;

    /*!*********************************************************************
    \brief
      Get ID of font from a string.
    \params
      name the name of the font
    \return
      the id of the font
    ************************************************************************/
    Graphics::gObjID  GetFontID(std::string const& name)const;

    /*!*********************************************************************
    \brief
      Gets the font scale to keep size uniform regardless of resolution
    \param fontID
      The ID of the font
    \return
      The scale of the font
    ************************************************************************/
    GLfloat GetFontScale(Graphics::gObjID fontID)const;

    GLfloat GetFontHeight(Graphics::gObjID fontID)const;

    GLuint const&     fontModel{ m_fontQuad };      // getter to font model
    GLuint const&     fontShader{ m_fontShader };   // getter to font shader
    
    /*!*********************************************************************
    \brief
      Gets the font lookup table from string to ID.
    \params
    \return
      read-only table for font lookups
    ************************************************************************/
    FontID_LT const&  GetFontLT()const;

    /*!*********************************************************************
    \brief
      Frees font information from both the cpu and gpu.
      Do not call Init in this class when done
    \return
    ************************************************************************/
    void FreeFonts();

    GLfloat GetTextWidth(std::string::const_iterator begin, std::string::const_iterator end, 
      Graphics::gObjID fontID, GLfloat scale);

    void    TextNewLines(std::string::const_iterator begin, std::string::const_iterator end, std::vector<size_t>& nlInfo,
      size_t currIdx = 0u)const;

    void    GetTextLinesInfo(Graphics::gObjID fontID, std::string const& text, 
      std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo>& nlInfo, FontAlign align = FontAlign::LEFT)const;
  };
}

#endif
