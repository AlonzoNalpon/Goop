#include <Graphics/Renderer/Renderer.h>
#include <include/def.h>
namespace
{
  enum
  {
    RENDER_CALLS_INITIAL_SIZE = 2048
  };
}

namespace Graphics::Rendering {
  Renderer::Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, ShaderCont const& shaderCont)
    : r_mdlContainer{ mdlContainer }, r_texManager{ texManager }, r_shaders{shaderCont}

  {
  }
  void Renderer::Init(size_t renderCallSize)
{
  m_renderCalls.reserve(renderCallSize);
}

void Renderer::RenderObject(gObjID mdl, SpriteData const& sprite)
{
  // Add the model data
  m_renderCalls.emplace_back(mdl, sprite);
}

void Renderer::Draw()
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
    glUseProgram(mdl.shader); // USE SHADER PROGRAM

    // Setting uniform variables
    constexpr GLint texPosLocation  = 0;  // Layout location for uTexPos
    constexpr GLint texDimsLocation = 1; // Layout location for uTexDims

    glUniform2f(texDimsLocation, obj.sprite.info.texDims.x, obj.sprite.info.texDims.y);
    glUniform2f(texPosLocation, obj.sprite.info.texCoords.x, obj.sprite.info.texCoords.y);


    glBindVertexArray(mdl.vaoid); // bind vertex array object to draw
    //glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt); // I leave this here as a reference for future optimizations
    glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt);
    glBindVertexArray(0);         // unbind vertex array object
    
    glUseProgram(0);        // UNUSE SHADER PROGRAM
    if (obj.sprite.texture != BAD_OBJ_ID)
    {
      glBindTextureUnit(7, 0);
    }
  }

  m_renderCalls.clear(); // reset
}

}