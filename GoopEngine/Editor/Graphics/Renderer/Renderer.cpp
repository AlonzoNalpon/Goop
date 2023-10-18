#include <Graphics/Renderer/Renderer.h>
#include <def.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp> // For quaternion rotations
#include <gtc/type_ptr.hpp>
#include <algorithm>
#include <functional>
namespace
{
  enum
  {
    RENDER_CALLS_INITIAL_SIZE = 2048
  };
}

namespace Graphics::Rendering {
  Renderer::Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, 
    ShaderCont const& shaderCont, Fonts::FontManager const& fontManager)
    : r_mdlContainer{ mdlContainer }, r_texManager{ texManager }, r_shaders{ shaderCont }, r_fontManager { fontManager }
  {
  }

  void Renderer::Init(Camera const& camera, size_t renderCallSize)
  {
    m_renderCalls.reserve(renderCallSize);
    m_camera = camera;
  }

  void Renderer::RenderObject(gObjID mdl, SpriteData const& sprite, Transform const&transform)
  {
    // Add the model data
    m_renderCalls.emplace_back(mdl, sprite, transform);
  }

  void Renderer::Draw()
  {
    std::sort(m_renderCalls.begin(), m_renderCalls.end(), DepthComp());
    glm::mat4 camViewProj{ m_camera.ViewProjMtx() };
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
      constexpr GLint uTexPosLocation   = 0;  // Layout location for uTexPos
      constexpr GLint uTexDimsLocation  = 1;  // Layout location for uTexDims
      constexpr GLint uViewMatLocation  = 2;  // Layout location for uViewProjMtx
      constexpr GLint uMdlTransLocation = 3;  // Layout location for uMdlMtx
      
      glUniform2f(uTexDimsLocation, obj.sprite.info.texDims.x, obj.sprite.info.texDims.y);
      glUniform2f(uTexPosLocation, obj.sprite.info.texCoords.x, obj.sprite.info.texCoords.y);
      // Pass the camera matrix
      glUniformMatrix4fv(uViewMatLocation, 1, GL_FALSE, glm::value_ptr(camViewProj));
      // Pass the model transform matrix
      glm::mat4 mdlXForm{ CalculateTransform(obj.transform)};
      glUniformMatrix4fv(uMdlTransLocation, 1, GL_FALSE, glm::value_ptr(mdlXForm));


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

  glm::mat4 Renderer::CalculateTransform(gVec3 const& scale, GLfloat rotation, gVec3 const& pos) const
  {
    glm::mat4 retval(1.f); // identity
    // Apply translation
    retval = glm::translate(retval, pos);

    // Apply rotation about z axis
    glm::quat rotationQuat = glm::angleAxis(rotation, glm::vec3(0.0f, 0.0f, 1.0f)); 
    retval = retval * glm::mat4_cast(rotationQuat);

    // Apply scaling
    retval = glm::scale(retval, scale);
    return retval;
  }

  glm::mat4 Renderer::CalculateTransform(Transform const& xForm) const
  {
    return CalculateTransform(xForm.scale, xForm.rotation, xForm.pos);
  }

}