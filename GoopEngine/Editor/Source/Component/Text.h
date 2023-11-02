#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/Def/GraphicsTypes.h>
namespace GE
{
	namespace Component
	{
		struct Text
		{
			std::string				m_text;
			Graphics::Colorf	m_clr;
			f32								m_scale;
			Graphics::gObjID  m_fontID{};
		};
	}
}

#endif
