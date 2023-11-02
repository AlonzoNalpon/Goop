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
			Graphics::Colorf	m_clr{ 1.f, 0.f, 1.f, 1.f };
			f32								m_scale{ 0.1f };
			Graphics::gObjID  m_fontID{};
		};
	}
}

#endif
