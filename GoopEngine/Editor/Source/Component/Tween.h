/*!*********************************************************************
\file   Tween.h
\author c.phua\@digipen.edu
\co-authors a.nalpon\@digipen.edu
\date   20-September-2023
\brief
	Component for interpolation.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
				Colorf m_spriteTint;
				Colorf m_spriteMult{1.f, 1.f, 1.f, 1.f};
				Colorf m_textColor;
				double m_duration;
				std::string m_animationEvent;

				Action() = default;
				Action(vec3 const& pos, vec3 const& scale, vec3 const& rot, double duration, std::string const& animEvent)
					: m_trans{ pos }, m_scale{ scale }, m_rot{ rot }, m_spriteTint{}, m_spriteMult{ 1.f, 1.f, 1.f, 1.f },
						m_textColor{}, m_duration { duration }, m_animationEvent{ animEvent } {}
				Action(vec3 const& pos, vec3 const& scale, vec3 const& rot, Colorf const& sprTint, Colorf const& sprMult,
					Colorf const& textCol, double duration = {}, std::string const& animEvent = {})
					: m_trans{ pos }, m_scale{ scale }, m_rot{ rot }, m_spriteTint{ sprTint }, m_spriteMult{ sprMult },
					m_textColor{ textCol }, m_duration{ duration }, m_animationEvent{ animEvent } {}
			};

			std::map<std::string, std::vector<Action>> m_tweens;
			double m_timeTaken;
			double m_timeElapsed;
			vec3 m_originalPos;
			vec3 m_originalScale;
			vec3 m_originalRot;
			Colorf m_originalSpriteTint;
			Colorf m_originalSpriteMult;
			Colorf m_originalTextColor;
			bool m_started;
			bool m_paused;
			bool m_loop;
			int m_step;
			std::string m_playing;

			/*!*********************************************************************
			\brief
				Default constructor
			************************************************************************/
			Tween() : m_timeTaken{ 0.0 }, m_timeElapsed{ 0.0 }, m_started{ false }, m_paused{ false }, m_step{ 0 } {}

			/*!*********************************************************************
			\brief
				Adding position into queue to calculate interpolation
			\param target
				New position to interpolate to.
			************************************************************************/
			void AddTween(std::string const& name, Action const& keyframe)
			{
				m_tweens[name].emplace_back(keyframe);
			}
			void AddTween(std::string const& name, Action&& keyframe)
			{
				m_tweens[name].emplace_back(std::move(keyframe));
			}
		};
	}
}
