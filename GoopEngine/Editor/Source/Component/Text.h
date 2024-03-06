/*!*********************************************************************
\file   Text.h
\author a.nalpon\@digipen.edu
\date   2-November-2023
\brief  This file contains the definition for the text component for
				ecs.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Fonts/TextObjGroup.h>
namespace GE
{
	namespace Component
	{
		/*!
		 * \struct Text
		 * \brief  
		 * .
		 */
		struct Text
		{
			std::string				m_text{"YOUR TEXT HERE"};			//!< text string (this literal is for ease of grading demo)
			Graphics::Colorf	m_clr{ 1.f, 0.f, 1.f, 1.f };	//!< color of text
			f32								m_scale{ 0.1f };							//!< scale of text
			Graphics::gObjID  m_fontID{};										//!< font ID of text
			f32								m_width{};										//!< width of text object (wrapping)
			// NON-SERIALIZED VARIABLES:
			Graphics::Fonts::TextObjGroup m_textInfo;					//!< rendering information only for runtime
			inline void SetColor(Graphics::Colorf clr) { m_clr = clr; }
			inline void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { SetColor({ r,g,b,a }); }
		};
	}
}

#endif
