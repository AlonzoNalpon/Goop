/*!*********************************************************************
\file   FontRenderer.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the FontRenderer class.
      It's still under construction
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <Graphics/Fonts/FontRenderer.h>
#include <Graphics/Fonts/Character.h>
namespace Graphics::Rendering
{
  FontRenderer::FontRenderer()
    : m_fontQuad(GenerateFontQuad())
  {
    ShaderInitCont fontShaders{ { GL_VERTEX_SHADER, "font.vert" }, {GL_FRAGMENT_SHADER, "font.frag"} };
    m_fontShader.CompileLinkValidate(fontShaders); // compile font shader
  }

  void FontRenderer::RenderText(glm::mat4 const& /*proj*/, std::string const& /*str*/, gVec2 /*pos*/, GLfloat /*scale*/, Colorf /*clr*/)
  {
    /*constexpr GLint U_PROJ{0}, U_TEXT{1}, U_CLR{2};
    glUseProgram(m_fontShader.GetHandle());
    glUniform3f(U_CLR, clr.r, clr.g, clr.b);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_fontQuad);

    for (auto c{ str.begin() }; c != str.end(), ++c)
    {
      Fonts::Character const& 
    }*/
  }

  GLuint FontRenderer::GenerateFontQuad()
  {
    GLuint vbo_hdl{}, vaoid{}; // vertex buffer object handle
    glCreateVertexArrays(1, &vaoid);
    glBindVertexArray(vaoid);

    glCreateBuffers(1, &vbo_hdl); // generate vbo
    constexpr GLsizeiptr VERTICES_COUNT{ 6 };
    // allocate uninitialized memory for vbo (data to be set for each char later on)
    glNamedBufferData(vbo_hdl, sizeof(float) * VERTICES_COUNT * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexArrayAttrib(vaoid, 0);
    glVertexArrayAttribFormat(vaoid, 0, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 0, 0);

    // Binding vbo to vao
    glVertexArrayVertexBuffer(vaoid, 0, vbo_hdl, 0, 4 * sizeof(GLfloat));

    // We won't be using indices for the quad ...
    glBindVertexArray(0); // All done. Unbind vertex array
    return vaoid;
  }
}
