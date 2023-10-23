#include <pch.h>
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
    ShaderCont const& shaderCont, Fonts::FontManager const& fontManager, DeferredInfo const& deferredInfo,
    Model const& vpQuadMdl)
    : r_mdlContainer{ mdlContainer }, r_texManager{ texManager }, r_shaders{ shaderCont }, r_fontManager { fontManager },
    r_deferredInfo{deferredInfo}, r_vpQuadMdl{vpQuadMdl}
  {
  }

  void Renderer::Init(Camera const& camera, size_t renderCallSize)
  {
    m_renderCalls.reserve(renderCallSize);
    m_camera = camera;
  }

  void Renderer::RenderObject(gObjID mdl, SpriteData const& sprite, GE::Math::dMat4 const& trans)
  {
    // Add the model data
    // Create a glm::mat4 and initialize it with values in a column-major order
    m_renderCalls.emplace_back(mdl, sprite, glm::mat4(
      static_cast<f32>(trans.At(0,0)), static_cast<f32>(trans.At(0,1)), static_cast<f32>(trans.At(0,2)), static_cast<f32>(trans.At(0,3)), // col 1
      static_cast<f32>(trans.At(1,0)), static_cast<f32>(trans.At(1,1)), static_cast<f32>(trans.At(1,2)), static_cast<f32>(trans.At(1,3)), // col 2
      static_cast<f32>(trans.At(2,0)), static_cast<f32>(trans.At(2,1)), static_cast<f32>(trans.At(2,2)), static_cast<f32>(trans.At(2,3)), // col 3
      static_cast<f32>(trans.At(3,0)), static_cast<f32>(trans.At(3,1)), static_cast<f32>(trans.At(3,2)), static_cast<f32>(trans.At(3,3)) // col 4
    )  );
  }

  void Renderer::Draw()
  {
    {
      /*
      std::sort(m_renderCalls.begin(), m_renderCalls.end(), DepthComp());
      glm::mat4 camViewProj{ m_camera.ViewProjMtx() };
      // Draw onto gbuffer (geometry + deferred pass)
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
        constexpr GLint uTexPosLocation = 0;  // Layout location for uTexPos
        constexpr GLint uTexDimsLocation = 1;  // Layout location for uTexDims
        constexpr GLint uViewMatLocation = 2;  // Layout location for uViewProjMtx
        constexpr GLint uMdlTransLocation = 3;  // Layout location for uMdlMtx

        glUniform2f(uTexDimsLocation, obj.sprite.info.texDims.x, obj.sprite.info.texDims.y);
        glUniform2f(uTexPosLocation, obj.sprite.info.texCoords.x, obj.sprite.info.texCoords.y);
        // Pass the camera matrix
        glUniformMatrix4fv(uViewMatLocation, 1, GL_FALSE, glm::value_ptr(camViewProj));
        // Pass the model transform matrix
        glm::mat4 mdlXForm{ CalculateTransform(obj.transform) };
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
      */
    }

    //std::sort(m_renderCalls.begin(), m_renderCalls.end(), DepthComp());
    glm::mat4 camViewProj{ m_camera.ViewProjMtx() };

    // Draw onto gbuffer (geometry pass)
    glBindFramebuffer(GL_FRAMEBUFFER, r_deferredInfo.gBuffer); // bind the G buffer

    for (auto const& obj : m_renderCalls)
    {
      constexpr GLuint TEXTURE_BINDING{ 7 };
      Model const& mdl{ r_mdlContainer[obj.mdl] };  
      
      glUseProgram(mdl.shader); // USE SHADER PROGRAM
      if (obj.sprite.texture != BAD_OBJ_ID)
      {
        Texture const& texObj{ r_texManager.GetTexture(obj.sprite.texture) };
        glBindTextureUnit(TEXTURE_BINDING, texObj.textureHandle);
        glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      }

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
      glm::mat4 const& mdlXForm{ obj.transform };
      glUniformMatrix4fv(uMdlTransLocation, 1, GL_FALSE, glm::value_ptr(mdlXForm));


      glBindVertexArray(mdl.vaoid); // bind vertex array object to draw
      //glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt); // I leave this here as a reference for future optimizations
      glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt);
      glBindVertexArray(0);         // unbind vertex array object
      
      glUseProgram(0);        // UNUSE SHADER PROGRAM
      if (obj.sprite.texture != BAD_OBJ_ID)
      {
        glBindTextureUnit(TEXTURE_BINDING, 0);
      }
    }
    // Deferred pass
    {
      constexpr GLuint POSITN_BINDING{ 0 };
      constexpr GLuint ALBEDO_BINDING{ 1 };
      constexpr GLuint NORMAL_BINDING{ 2 };

      glBindTextureUnit(POSITN_BINDING, r_deferredInfo.posTexture);
      glBindTextureUnit(ALBEDO_BINDING, r_deferredInfo.albedoTexture);
      glBindTextureUnit(NORMAL_BINDING, r_deferredInfo.normalTexture);

      glBindFramebuffer(GL_FRAMEBUFFER, 0); // remove
      //glBindFramebuffer(GL_FRAMEBUFFER, r_framebuffer); // bind the final framebuffer
      Model const& mdl = r_vpQuadMdl;
      glUseProgram(mdl.shader);

      // UNIFORMS
      glBindVertexArray(mdl.vaoid); // bind vertex array object to draw

      glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt);

      glBindTextureUnit(POSITN_BINDING, 0);
      glBindTextureUnit(ALBEDO_BINDING, 0);
      glBindTextureUnit(NORMAL_BINDING, 0);
      glUseProgram(0);        // UNUSE SHADER PROGRAM
      glBindVertexArray(0);         // unbind vertex array object
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

  //glm::mat4 Renderer::CalculateTransform(Transform const& xForm) const
  //{
  //  return CalculateTransform(xForm.scale, xForm.rotation, xForm.pos);
  //}

}