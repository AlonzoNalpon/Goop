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

namespace GE::Graphics::Rendering {
  Renderer::Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager,
    ShaderCont const& shaderCont, Fonts::FontManager const& fontManager, GLint const& vpWidth, GLint const& vpHeight,
    std::map < gObjID, FrameBufferInfo> const& frameBuffers)
    : r_mdlContainer{ mdlContainer }, r_texManager{ texManager }, r_shaders{ shaderCont }, r_fontManager{ fontManager },
    r_vpWidth{ vpWidth }, r_vpHeight{ vpHeight }, r_frameBuffers{ frameBuffers }
  {
  }

  void Renderer::Init(Camera const& camera, gObjID lineMdlID, size_t renderCallSize)
  {
    m_spriteRenderCalls.reserve(renderCallSize);
    m_camera = camera;
    m_lineMdlObj = lineMdlID;
  }

  void Renderer::RenderObject(SpriteData const& sprite, GE::Math::dMat4 const& trans)
  {
    m_renderData.emplace_back(RenderDataTypes::SPRITE, m_spriteRenderCalls.size(), static_cast<f32>(trans.At(3, 2)));
    // Add the model data
    // Create a glm::mat4 and initialize it with values in a column-major order
    m_spriteRenderCalls.emplace_back(sprite, glm::mat4(
      static_cast<f32>(trans.At(0,0)), static_cast<f32>(trans.At(0,1)), static_cast<f32>(trans.At(0,2)), static_cast<f32>(trans.At(0,3)), // col 1
      static_cast<f32>(trans.At(1,0)), static_cast<f32>(trans.At(1,1)), static_cast<f32>(trans.At(1,2)), static_cast<f32>(trans.At(1,3)), // col 2
      static_cast<f32>(trans.At(2,0)), static_cast<f32>(trans.At(2,1)), static_cast<f32>(trans.At(2,2)), static_cast<f32>(trans.At(2,3)), // col 3
      static_cast<f32>(trans.At(3,0)), static_cast<f32>(trans.At(3,1)), static_cast<f32>(trans.At(3,2)), static_cast<f32>(trans.At(3,3)) // col 4
    )  );
  }

  void Renderer::RenderFontObject(gVec3 pos, GLfloat scale, std::string const& str, 
    std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo> const& newLines, Colorf color, gObjID fontID)
  {
    m_renderData.emplace_back(RenderDataTypes::FONT, m_fontRenderCalls.size(), pos.z);
    m_fontRenderCalls.emplace_back(pos, scale * r_fontManager.GetFontScale(fontID), str, newLines, color, fontID);
  }

  void Renderer::RenderLineDebug(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf const& clr)
  {
#ifndef NO_IMGUI
    m_lineRenderCalls.emplace_back(startPt, endPt, clr);
#else
    UNREFERENCED_PARAMETER(startPt);
    UNREFERENCED_PARAMETER(endPt);
    UNREFERENCED_PARAMETER(clr);
#endif
  }

  void Renderer::Draw(Camera& camera)
  {
    Model const& mdl{ r_mdlContainer.front() };
    constexpr GLint uTexPosLocation   = 0;  // Layout location for uTexPos
    constexpr GLint uTexDimsLocation  = 1;  // Layout location for uTexDims
    constexpr GLint uViewMatLocation  = 2;  // Layout location for uViewProjMtx
    constexpr GLint uMdlTransLocation = 3;  // Layout location for uMdlMtx
    constexpr GLint uTintLocation     = 5;  // Layout location for uTint
    constexpr GLint uMultiplyLocation = 6;  // layout location for uClrMult
    // Draw
    glm::mat4 const& camViewProj{ camera.GetPersMtx() };
    glUseProgram(r_mdlContainer.front().shader); // USE SHADER PROGRAM
    glBindVertexArray(r_mdlContainer.front().vaoid); // bind vertex array object to draw
    // Pass the camera matrix
    glUniformMatrix4fv(uViewMatLocation, 1, GL_FALSE, glm::value_ptr(camViewProj));
    m_prevMode = RenderDataTypes::SPRITE;

    for (RenderData const& curr : m_renderData)
    {
      // We do rendering based on depth (excluding lines which come later)
      bool const newMode{ curr.type != m_prevMode };
      switch (curr.type)
      {
      case RenderDataTypes::SPRITE:
      {
        if (newMode)
        {
          glBindVertexArray(r_mdlContainer.front().vaoid); // bind vertex array object to draw
          glUseProgram(r_mdlContainer.front().shader); // USE SHADER PROGRAM 
        }

        SpriteRenderData const& obj{ m_spriteRenderCalls[curr.element] };

        Texture const& texObj{ r_texManager.GetTexture(obj.sprite.texture) };

        // BIND TEXTURE AND SETUP MODE
        glBindTextureUnit(7, texObj.textureHandle);
        glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTextureParameteri(texObj.textureHandle, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // Setting uniform variables
        glUniform2f(uTexDimsLocation, obj.sprite.info.texDims.x, obj.sprite.info.texDims.y);
        glUniform2f(uTexPosLocation, obj.sprite.info.texCoords.x, obj.sprite.info.texCoords.y);
        // Pass the model transform matrix
        glm::mat4 const& mdlXForm{ obj.transform };
        glUniformMatrix4fv(uMdlTransLocation, 1, GL_FALSE, glm::value_ptr(mdlXForm));
        // Pass the alpha value
        Colorf const& clrTint{ obj.sprite.info.tint };
        Colorf const& clrMult{ obj.sprite.info.multiply };
        glUniform4f(uTintLocation, clrTint.r, clrTint.g, clrTint.b, clrTint.a);
        glUniform4f(uMultiplyLocation, clrMult.r, clrMult.g, clrMult.b, clrMult.a);

        glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt); // draw the object

        glBindTextureUnit(7, 0); // unuse the texture
      }
        break;

      case RenderDataTypes::FONT:
      {
        if (newMode)
        {
          glUseProgram(r_fontManager.fontShader);
        }
        FontRenderData const& obj{ m_fontRenderCalls[curr.element] };
        DrawFontObj(obj.str, obj.newLines, obj.position, obj.scale, obj.clr, obj.fontID, camera);
      }
        break;
      default:
        break;
      }

      m_prevMode = curr.type;
    }

   

    
    glBindVertexArray(0);         // unbind vertex array object
    glUseProgram(0);        // UNUSE SHADER PROGRAM
    glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

  


    // RENDERING LINES FRO DEBUGGING
    {
      constexpr GLint uEndPtsLocation = 0;
      constexpr GLint uColorLocation = 1;
      constexpr GLint uViewProjMtxLocation = 2;
      auto const& lineMdl{ r_mdlContainer[m_lineMdlObj]};
      glUseProgram(lineMdl.shader);

      // Pass the camera matrix
      glUniformMatrix4fv(uViewProjMtxLocation, 1, GL_FALSE, glm::value_ptr(camViewProj));
      for (auto const& obj : m_lineRenderCalls)
      {
        glLineWidth(1.f);
        
        glBindVertexArray(lineMdl.vaoid);

        // Set the uniform values using the layout locations
        glUniform4f(uEndPtsLocation, static_cast<GLfloat>(obj.startPt.x), 
          static_cast<GLfloat>(obj.startPt.y), 
          static_cast<GLfloat>(obj.endPt.x), 
          static_cast<GLfloat>(obj.endPt.y));
        glUniform3f(uColorLocation, obj.clr.r, obj.clr.g, obj.clr.b);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
      }
      glUseProgram(0); // we're done
    }
  }

  void Renderer::DrawFontObj(std::string const& str, std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo> const& newLines,
    gVec2 pos, GLfloat scale, Colorf const& clr, Graphics::gObjID fontID, Camera& camera)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    constexpr GLint uViewMatLocation{ 0 };
    constexpr GLint uColorLocation{ 2 };
    auto const& fontMap = r_fontManager.GetFontMap(fontID);
    GLfloat fontHeight  = r_fontManager.GetFontHeight(fontID) * scale;
    GLfloat const oldXPos = pos.x;
    glm::mat4 camViewProj{ camera.GetPersMtx() };

    // ASSUMING THAT WE USED THE SHADER BEFORE FUNCTION CALL
    // pass the color
    glUniform4f(uColorLocation, clr.r, clr.g, clr.b, clr.a);
    // Pass the camera matrix
    glUniformMatrix4fv(uViewMatLocation, 1, GL_FALSE, glm::value_ptr(camViewProj));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(r_fontManager.fontModel);

    // iterate threough the str
    size_t currIdx{}; // curr idx of the string
    size_t currLineIdx{}; // first position of curr line
    auto lineInfoIt{ newLines.cbegin() };
    {
      //pos.y -= fontHeight*0.5f; // update the position y value to perform "newline"
      pos.x += scale * lineInfoIt->baseOffset;     // and update x position based on alignment data
      ++lineInfoIt;
    }
    for (auto strIt{str.cbegin()} ; strIt != str.cend(); ++strIt, ++currIdx)
    {
      char ch = *strIt;
      if (lineInfoIt != newLines.cend()
        && currIdx == lineInfoIt->idx - 1
        && currLineIdx != lineInfoIt->idx) // if there's a possible new line
      {
        pos.y -= fontHeight; // update the position y value to perform "newline"
        pos.x = oldXPos + scale * lineInfoIt->baseOffset;     // and update x position based on alignment data

        currLineIdx = lineInfoIt->idx; // update curr index
        ++lineInfoIt; // go to the next element of newline indexes
        continue; // skip rendering this character
      }

      Fonts::Character const& currGlyph{ fontMap.at(ch) };
      GLfloat xpos = pos.x + currGlyph.bearing.x * scale;
      GLfloat ypos = pos.y - (currGlyph.size.y - currGlyph.bearing.y) * scale;

      GLfloat w = currGlyph.size.x * scale;
      GLfloat h = currGlyph.size.y * scale;
      // update the VBO
      GLfloat verts[6][4] = {
        // TRIANGLE 1
        { xpos,     ypos + h,   0.0f, 0.0f }, // top left
        { xpos,     ypos,       0.0f, 1.0f }, // bottom left
        { xpos + w, ypos,       1.0f, 1.0f }, // bottom right
        // TRIANGLE 2
        { xpos,     ypos + h,   0.0f, 0.0f }, // top left
        { xpos + w, ypos,       1.0f, 1.0f }, // bottom right
        { xpos + w, ypos + h,   1.0f, 0.0f }  // top right
      };

      // Rendering the glyph tex on quad
      glBindTexture(GL_TEXTURE_2D, currGlyph.textureID);
      // update vbo with new vertex data
      glBindBuffer(GL_ARRAY_BUFFER, r_fontManager.fontModel);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // Render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // advance cursor for next glyph

      //advance is number of 1/64 pixels
      pos.x += (currGlyph.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
  }

  Camera& Renderer::GetCamera()
  {
    return m_camera;
  }

  void Renderer::PreRenderSetup()
  {
    std::sort(m_renderData.begin(), m_renderData.end(), [](RenderData const& a, RenderData const& b) { return a.zVal < b.zVal; });
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

  void Renderer::ClearRenderContainers()
  {

    m_spriteRenderCalls.clear(); // reset
    m_fontRenderCalls.clear(); // reset fonts
    m_lineRenderCalls.clear(); // reset debug
    m_renderData.clear(); // reset the renderdata
  }

  //glm::mat4 Renderer::CalculateTransform(Transform const& xForm) const
  //{
  //  return CalculateTransform(xForm.scale, xForm.rotation, xForm.pos);
  //}

}