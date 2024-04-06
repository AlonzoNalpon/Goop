/*!*********************************************************************
\file   BoxCollider.h
\author c.phua\@digipen.edu
\date   16-September-2023
\brief
	Component for AABB collider box.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <Graphics/GraphicsEngine.h>

namespace GE
{
	namespace Component
	{
		struct BoxCollider
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			BoxCollider() = default;

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			BoxCollider(Math::dVec2 const& center, double width, double height) : m_width{ width }, m_height{ height },
				m_min{ center.x - width / 2.0f, center.y - height / 2.0f },
				m_max{ center.x + width / 2.0f, center.y + height / 2.0f }, m_center{ center },
				m_collided{ nullptr },
				m_mouseCollided{ false }
#ifndef IMGUI_DISABLE
				,m_render{ false }
#endif
			{
				m_collided.clear();
			}

			double m_width;
			double m_height;
			Math::dVec2 m_min;
			Math::dVec2 m_max;
			Math::dVec2 m_center;
			std::set<BoxCollider*> m_collided; //a set list of entities' pointers that current entity is collided with
			bool m_mouseCollided;
#ifndef IMGUI_DISABLE
			bool m_render;
#endif

			/*!*********************************************************************
			\brief
				Drawing of collision box
			************************************************************************/
			void Render()
			{
				// Idk how to tell if i'm no longer collided with something else. Please do if free (I DID IT)
				Graphics::Colorf boxColor = 
					(!m_collided.empty() ? // THIS IS SUPPOSED TO BE BOOL THAT INDICATES IS COLLIDING
					Graphics::Colorf{1.f, 0.f, 0.f, 1.f}			// collided : red
				: Graphics::Colorf{ 0.f, 1.f, 0.f, 1.f });	// not collided: green

				auto& gEngine{ Graphics::GraphicsEngine::GetInstance() };
				gEngine.DrawLine(m_min, { m_max.x, m_min.y }, boxColor);
				gEngine.DrawLine({ m_max.x, m_min.y }, m_max, boxColor);
				gEngine.DrawLine(m_max, { m_min.x, m_max.y }, boxColor);
				gEngine.DrawLine({ m_min.x, m_max.y }, m_min, boxColor);
			}
		};
	}
}