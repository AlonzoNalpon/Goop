/*!*********************************************************************
\file   Tween.h
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Component for interpolation.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <vector>
//#include <Graphics/Def/GraphicsTypes.h>
namespace GE
{
	namespace Component
	{
		using namespace Graphics;
		struct Tween
		{
			using vec3 = GE::Math::dVec3;
			struct Action
			{
				vec3 m_trans;
				vec3 m_scale;
				vec3 m_rot;
				Colorf m_spriteColor;
				Colorf m_textColor;
				double m_duration;
				std::string m_animationEvent;
			};

			std::map<std::string, std::vector<Action>> m_tweens;
			double m_timeTaken;
			double m_timeElapsed;
			vec3 m_originalPos;
			vec3 m_originalScale;
			vec3 m_originalRot;
			Colorf m_originalSpriteColor;
			Colorf m_originalTextColor;
			bool m_started;
			bool m_paused;
			bool m_loop;
			int m_step;
			std::string m_playing;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Tween() : m_timeTaken{ 0.0 }, m_timeElapsed{ 0.0 }, m_started{ false }, m_paused{ false }, m_step{ 0 } {}

			/*!*********************************************************************
			\brief
				Adding position into queue to calculate interpolation
			\param target
				New position to interpolate to.
			************************************************************************/
			void AddTween(std::string name, Action keyframe)
			{
				m_tweens[name].emplace_back(keyframe);
			}
		};
	}
}
