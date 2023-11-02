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
			std::string				m_text{"YOUR TEXT HERE"};
			Graphics::Colorf	m_clr{ 1.f, 0.f, 1.f, 1.f };
			f32								m_scale{ 0.1f };
			Graphics::gObjID  m_fontID{};
		};
	}
}

#endif
