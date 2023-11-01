#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <string>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/Def/GraphicsTypes.h>
namespace GE
{
	namespace Component
	{
		struct TextObject
		{
			std::string				m_text;
			Graphics::Colorf	m_clr;

		};
	}
}

#endif
