/*!*********************************************************************
\file   FontRenderer.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the FontRenderer class.
        This class is meant for rendering fonts with opengl calls
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
#include <string>
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/ShaderProgram.h>

namespace GE::Graphics::Rendering
{
  /*!
   * \class FontRenderer
   * \brief  
   * This class is responsible for rendering fonts using opengl api.
   */
  class FontRenderer
  {
  public:
    FontRenderer();
    void RenderText(glm::mat4 const& proj, std::string const& str, gVec2 pos, GLfloat scale, Colorf clr);
  private:
    ShaderProgram m_fontShader; //!< shader for rendering the fonts
    GLuint        m_fontQuad;   //!< quad for rendering the fonts

    /*!*********************************************************************
    \brief
      Generates a quad to be used with the fonts. Requires opengl to be
      initialized before calling
    \params
    \return
      
    ************************************************************************/
    GLuint GenerateFontQuad();
    // In the future, one can add more members for advanced features ...
  };
}

#endif
