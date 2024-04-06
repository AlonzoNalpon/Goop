/*!*********************************************************************
\file   Text.h
\author a.nalpon\@digipen.edu
\date   2-November-2023
\brief  This file contains the definition for the text component for
				ecs.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/Def/GraphicsTypes.h>
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
			
			using TextAlignment = GE::Graphics::Fonts::FontManager::FontAlign;

			std::string					m_text{"YOUR TEXT HERE"};			//!< text string (this literal is for ease of grading demo)
			Graphics::Colorf		m_clr{ 1.f, 0.f, 1.f, 1.f };	//!< color of text
			f32									m_scale{ 0.1f };							//!< scale of text
			Graphics::gObjID		m_fontID{};										//!< font ID of text
			TextAlignment				m_alignment{ TextAlignment::LEFT };		// NOT SERIALIZED AT THE MOMENT
			// NON-SERIALIZED VARIABLES:
			bool								m_dirty{true}; // DO NOT EVER SERIALIZE

			std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo> m_textLinesInfo;	 // DO NOT SERIALIZE?
			
			/*!*********************************************************************
			\brief
			  Sets the color of the text component
			\param clr
				The color to set in the form of a Graphics::Colorf object
			************************************************************************/
			inline void SetColor(Graphics::Colorf clr) { m_clr = clr; }

			/*!*********************************************************************
			\brief
				Sets the color of the text component
			\param r
				The red component of the color
			\param g
				The green component of the color
			\param b
				The blue component of the color
			\param a
				The alpha component of the color
			************************************************************************/
			inline void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { SetColor({ r,g,b,a }); }
		};
	}
}

#endif
