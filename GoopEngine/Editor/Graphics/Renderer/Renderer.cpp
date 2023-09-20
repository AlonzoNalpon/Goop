#include <Graphics/Renderer/Renderer.h>
#include <include/def.h>
namespace
{
  enum
  {
    RENDER_CALLS_INITIAL_SIZE = 2048
  };
}

Graphics::Renderer::Renderer::Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, 
  Graphics::GraphicsEngine::ShaderCont const& shaderCont)
  : r_mdlContainer{mdlContainer}, r_texManager{texManager}, r_shaders{shaderCont}
{
}

void Graphics::Renderer::Renderer::Init()
{
  m_renderCalls.reserve(2048);
}

void Graphics::Renderer::Renderer::RenderObject(gObjID mdl, SpriteData const& sprite)
{
  // Add the model data
  m_renderCalls.emplace_back(mdl, sprite);
}

void Graphics::Renderer::Renderer::Draw()
{
  // Draw

  for (auto const& obj : m_renderCalls)
  {
    Model const& mdl{ r_mdlContainer[obj.mdl] };
    if (obj.sprite.texture != BAD_OBJ_ID)
    {
      Texture const& texObj{ r_texManager.GetTexture(obj.sprite.texture) };
      glBindTextureUnit(7, texObj.textureHandle);
      glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    glUseProgram(mdl.shader);

    // Setting uniform variables
    constexpr GLint texPosLocation  = 0;  // Layout location for uTexPos
    constexpr GLint texDimsLocation = 1; // Layout location for uTexDims

    glUniform2f(texDimsLocation, static_cast<GLfloat>(obj.sprite.info.width), static_cast<GLfloat>(obj.sprite.info.height));
    glUniform2f(texPosLocation, obj.sprite.info.texCoords.x, obj.sprite.info.texCoords.y);


    glBindVertexArray(mdl.vaoid);
    //glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt); // I leave this here as a reference for future optimizations
    glDrawArrays(GL_LINES, 0, mdl.draw_cnt);
    
    glBindVertexArray(0);
    glUseProgram(0);
    if (obj.sprite.texture != BAD_OBJ_ID)
    {
      glBindTextureUnit(7, 0);
    }
  }

  m_renderCalls.clear(); // reset
}
