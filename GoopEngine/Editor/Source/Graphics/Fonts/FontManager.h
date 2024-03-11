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
#include <Graphics/Fonts/TextObjGroup.h>
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
    FT_Library        m_library;
    FontMapFontMap    m_fonts;      // !< all loaded fonts' glyphs stored here
    FontID_LT         m_fontIDLT;     // !< lookup table to get font ID from string
    FontName_LT       m_fontNameLT; // !< lookup table to get name from ID
    GLuint            m_fontQuad;
    GLuint            m_fontShader;
    FontScales        m_fontScales;
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

    void              UpdateTextInfo(TextObjGroup& group);
  };
}

#endif
