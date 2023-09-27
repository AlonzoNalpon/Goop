#include <Graphics/Fonts/FontManager.h>
#include <Debugger/Exception/Exception.h>
namespace Graphics::Fonts
{
  void FontManager::Init()
  {
    FT_Error error{ FT_Init_FreeType(&library) };
    if (error)
    {
      throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Failed to initialize freetype!"));
    }
  }

  //bool FontManager::LoadFont(std::string const& name, std::string const& path, GLint fontSize)
  //{
  //  FT_Face face;
  //  if (FT_New_Face(library, path.c_str(), 0, &face))
  //  {
  //    throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_ERROR, ErrMsg("Failed to load font: " + path));
  //  }
  //  FT_Set_Pixel_Sizes(face, 0, 48); // the width is automatically set based on height

  //  
  //  // Add this font
  //  m_fonts.push_back(std::make_pair(name, std::map<uChar, Character>()));
  //  FontMap& fontMap{ m_fonts.back().second };

  //  // disable byte-alignment restriction (using only one byte instead of 4)
  //  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
  //  constexpr uChar MAX_CHARS{ 128 };
  //  for (uChar c{}; c < MAX_CHARS; ++c)
  //  {
  //    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
  //    {
  //      throw GE::Debug::Exception<FontManager>(GE::Debug::LEVEL_ERROR, ErrMsg("Failed to load glyph: " + c));
  //      continue;
  //    }

  //    // Generate a texture
  //    GLuint texHdl;
  //    glCreateTextures(GL_TEXTURE_2D, 1, &texHdl);
  //    // allocate memory for the texture
  //    glTextureStorage2D(texHdl, 1, GL_RED, face->glyph->bitmap.width, 
  //      face->glyph->bitmap.rows);
  //    // assign the texture data
  //    glTextureSubImage2D(
  //      texHdl, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows,
  //      GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
  //    );
  //    // set other texture options
  //    glTextureParameteri(texHdl, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //    glTextureParameteri(texHdl, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //    glTextureParameteri(texHdl, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //    glTextureParameteri(texHdl, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //    
  //    // Now store character
  //    fontMap[c] =
  //    Character{
  //      texHdl,
  //      {face->glyph->bitmap.width, face->glyph->bitmap.rows},
  //      {face->glyph->bitmap_left, face->glyph->bitmap_top},
  //      static_cast<GLuint>(face->glyph->advance.x)
  //    };
  //    FT_Done_Face(face); // we've gotten all the textures: we're done
  //    FT_Done_FreeType(library);
  //  }
  //  return true;
  //}
  //Character const& FontManager::GetCharacter(gObjID font, uChar ch)
  //{
  //  return m_fonts[font][ch]
  //}
}
