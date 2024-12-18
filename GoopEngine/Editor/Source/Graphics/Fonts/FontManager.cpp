/*!*********************************************************************
\file   FontManager.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the font manager class
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/Fonts/FontManager.h>
#include <DebugTools/Exception/Exception.h>
namespace GE::Graphics::Fonts
{
  void FontManager::Init(GLuint _fontShader, GLuint _fontQuad)
  {
    m_fontQuad = _fontQuad;
    m_fontShader = _fontShader;
    FT_Error error{ FT_Init_FreeType(&m_library) };
    if (error)
    {
      throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Failed to initialize freetype!"));
    }
  }


  void FontManager::LoadFont(std::string const& name, std::string const& path, GLint fontSize)
  {
    FT_Face face;
    // (makes all scaling identical across different fontSizes)
    GLfloat constexpr inverseDefSize{ 1.f/256.f }; // to multiply with size to get inverse scale 
    if (FT_New_Face(m_library, path.c_str(), 0, &face))
    {
      throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_ERROR, ErrMsg("Failed to load font: " + path));
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize); // the width is automatically set based on height

    
    // Add this font and get reference to the map
    Graphics::gObjID newID{ m_fonts.size() };
    
    // ADDING LOOKUP TABLE ENTRIES
    {
      // name
      auto nameIt{ m_fontIDLT.find(name) };
      if (nameIt != m_fontIDLT.end())
      {
        GE::Debug::ErrorLogger::GetInstance().LogMessage(
          "FontManager: Font " + name + " already exists, replacing texture.");
        return;
        /*throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_ERROR, 
          ErrMsg((std::string{"Font of this name already exists: " + name} + " in ") + path));*/
      }

      // If this point is reached, the font can be added!
      m_fontNameLT[newID] = name; // lookup table for font name
      m_fontIDLT[name] = newID;   // lookup table for fontID
    }
    FontMap& fontMap{ m_fonts[newID] }; // get reference to newly created map

    m_fontScales[newID] = 1 / (fontSize * inverseDefSize);
    //GLfloat const fontScale{ m_fontScales[newID] };
    GLfloat maxFontHeight{};
    // disable byte-alignment restriction (using only one byte instead of 4)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    constexpr uChar MAX_CHARS{ 128 };
    for (uChar c{}; c < MAX_CHARS; ++c)
    {
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_ERROR, ErrMsg("Failed to load glyph: " + c));
        continue;
      }
      
      // LOOK AT THIS ACTIVE PROCESSOR BLOCK INSTEAD OF THE INACTIVE ONE
#if 1
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
      );
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // Now store character
      fontMap[c] =
        Character{
          texture,
          {face->glyph->bitmap.width, face->glyph->bitmap.rows},
          {face->glyph->bitmap_left, face->glyph->bitmap_top},
          static_cast<GLuint>(face->glyph->advance.x)
      };
        if (c > 32 && c < 127)
      {
        GLfloat currHeight = static_cast<GLfloat>(face->glyph->bitmap_top);
        maxFontHeight = (currHeight > maxFontHeight) ? currHeight : maxFontHeight;
      }
#else
      // Generate a texture
      GLuint texHdl;
      glCreateTextures(GL_TEXTURE_2D, 1, &texHdl);
      // allocate memory for the texture
      glTextureStorage2D(texHdl, 1, GL_RED, face->glyph->bitmap.width,
        face->glyph->bitmap.rows);
      // assign the texture data
      glTextureSubImage2D(
        texHdl, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows,
        GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
      );
      // set other texture options
      glTextureParameteri(texHdl, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTextureParameteri(texHdl, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTextureParameteri(texHdl, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTextureParameteri(texHdl, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Now store character
      fontMap[c] =
        Character{
          texHdl,
          {face->glyph->bitmap.width, face->glyph->bitmap.rows},
          {face->glyph->bitmap_left, face->glyph->bitmap_top},
          static_cast<GLuint>(face->glyph->advance.x)
      };
#endif
    }    
    m_fontHeight[newID] = maxFontHeight;
          // What is this commented code: Cleanup code to be implemented elsewhere! 
    //FT_Done_Face(face); // we've gotten all the textures: we can clear freetype's resources
    //FT_Done_FreeType(m_library);
  }
      
  typename FontManager::FontMap const& FontManager::GetFontMap(Graphics::gObjID fontID)const
  {
    return m_fonts.at(fontID);
  }

  std::string FontManager::GetFontName(Graphics::gObjID fontName) const
  {
    return m_fontNameLT.at(fontName);
  }

  Graphics::gObjID FontManager::GetFontID(std::string const& name) const
  {
    return m_fontIDLT.at(name);
  }
  GLfloat FontManager::GetFontScale(Graphics::gObjID fontID) const
  {
    return m_fontScales.at(fontID);
  }
  GLfloat FontManager::GetFontHeight(Graphics::gObjID fontID) const
  {
    return m_fontHeight.at(fontID);
  }
  FontManager::FontID_LT const& FontManager::GetFontLT() const
  {
    return m_fontIDLT;
  }

  void FontManager::FreeFonts()
  {
    for (auto const& fontIt : m_fonts)
    {
      for (auto const& fontChar : fontIt.second)
      {
        glDeleteTextures(1, &fontChar.second.textureID);
      }
    }
    m_fontIDLT.clear();
    m_fonts.clear();
    m_fontScales.clear();
  }

  GLfloat FontManager::GetTextWidth(std::string::const_iterator begin, std::string::const_iterator end, Graphics::gObjID fontID, GLfloat scale)
  {
    GLfloat width{}; //TODO
    FontMap const& currFont{ m_fonts[fontID] };
    GLfloat fontScale{ m_fontScales[fontID] * scale };
    while (begin != end)
    {
      auto const& glyph = currFont.at(*begin);
      width += glyph.advance * fontScale;
      ++begin;
    }
    return width;
  }

  void FontManager::TextNewLines(std::string::const_iterator begin, std::string::const_iterator end, std::vector<size_t>& nlInfo,
    size_t currIdx)const
  {
    while (begin != end)
    {
      if (*begin == '\n')
        nlInfo.emplace_back(currIdx);
      ++currIdx;
      ++begin;
    }
  }

  void FontManager::GetTextLinesInfo(Graphics::gObjID fontID, std::string const& text, 
    std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo>& nlInfo, FontAlign align) const
  {
    size_t currIdx{};
    nlInfo.clear();
    nlInfo.emplace_back(currIdx++, 0.f); // the first line
    auto const& currFont{ m_fonts.at(fontID) };

    for (auto ch : text)
    {
      if (ch == '\n') // if a newline is encountered
      {
        nlInfo.emplace_back(currIdx, 0.f); // we now add another line to update
        ++currIdx;
        continue; // skip the newline character
      }
      GLfloat const width{ static_cast<float>(currFont.at(ch).advance >> 6) };

      switch (align)
      {
      case FontAlign::LEFT:
        // nothing
        break;
      case FontAlign::RIGHT:
        nlInfo.back().baseOffset -= width;
        break;
      case FontAlign::CENTER:
        nlInfo.back().baseOffset -= width * 0.5f;
        break;
      }
      ++currIdx;
    }
  }
}
