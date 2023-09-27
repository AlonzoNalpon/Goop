#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
#include <string>
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/ShaderProgram.h>

namespace Graphics::Rendering
{
  class FontRenderer
  {
  public:
    FontRenderer();
    void RenderText(glm::mat4 const& proj, std::string const& str, gVec2 pos, GLfloat scale, Colorf clr);
  private:
    ShaderProgram m_fontShader; //!< shader for rendering the fonts
    GLuint        m_fontQuad;   //!< quad for rendering the fonts
    GLuint GenerateFontQuad();
    // In the future, one can add more members for advanced features ...
  };
}

#endif
