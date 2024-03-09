/*!*********************************************************************
\file   TweenSystem.cpp
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Uses tweening to update entity's position.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#include <pch.h>
#include <Systems/TweenSystem/TweenSystem.h>
#include <Component/Tween.h>
#include <Component/Transform.h>
#include <math.h>

using vec3 = GE::Math::dVec3;

using namespace GE::ECS;
using namespace GE::Systems;
using namespace GE::Component;

void TweenSystem::FixedUpdate()
{
	static auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();

	double dt = GE::FPS::FrameRateController::GetInstance().GetFixedDeltaTime();

	for (Entity entity : GetUpdatableEntities()) 
	{
		Tween* tween = m_ecs->GetComponent<Tween>(entity);

		if (tween->m_tweens.size() == 0)
		{
			continue;
		}
		if (tween->m_paused)
		{
			continue;
		}

		Transform* trans = m_ecs->GetComponent<Transform>(entity);
		Sprite* sprite = m_ecs->HasComponent<Sprite>(entity) ? m_ecs->GetComponent<Sprite>(entity) : nullptr;
		Text* text = m_ecs->HasComponent<Text>(entity) ? m_ecs->GetComponent<Text>(entity) : nullptr;
		if (tween->m_tweens.find(tween->m_playing) == tween->m_tweens.end())
			continue;
		// Find the tween being played in the map, then find the step being played of the tween
		auto [target, scale, rot, spriteColor, textColor, duration, scriptName] 
			= tween->m_tweens[tween->m_playing][tween->m_step];

		if (tween->m_timeElapsed > duration)
		{
			tween->m_timeElapsed -= duration;
			tween->m_originalPos = target;
			tween->m_originalScale = scale;
			tween->m_originalRot = rot;
			tween->m_originalSpriteColor = spriteColor;
			tween->m_originalTextColor = textColor;
			// Get Scripts
			auto* scripts = m_ecs->GetComponent<GE::Component::Scripts>(entity);
			if (scripts)
			{
				// Find if scripts exist
				auto script = scripts->Get(scriptName);
				if (script)
				{
					// Call method if exist
					auto method = mono_class_get_method_from_name(script->m_scriptClass, "PlayEvent", 1);
					if (method)
					{
						void* args{&entity};
						mono_runtime_invoke(method, script->m_classInst, &args, nullptr);
					}
				}
			}

			double normalisedTime = tween->m_timeElapsed / duration;
			trans->m_pos = Tweening(tween->m_originalPos, target, normalisedTime);
			trans->m_scale = Tweening(tween->m_originalScale, scale, normalisedTime);
			trans->m_rot = Tweening(tween->m_originalRot, rot, normalisedTime);
			if (sprite)
			{
				sprite->m_spriteData.SetTint(
					Tweening(tween->m_originalSpriteColor, spriteColor, normalisedTime));
			}
			if (text)
			{
				text->SetColor(
					Tweening(tween->m_originalTextColor, textColor, normalisedTime));
			}
			++tween->m_step;
			if (tween->m_step >= tween->m_tweens[tween->m_playing].size())
			{
				tween->m_step = 0;
				if (!tween->m_loop)
				{
					tween->m_playing = "";
				}
			}
		}
		else
		{
			if (!tween->m_started)
			{
				tween->m_originalPos = trans->m_pos;
				tween->m_originalScale = trans->m_scale;
				tween->m_originalRot = trans->m_rot;
				if (sprite)
					tween->m_originalSpriteColor = sprite->m_spriteData.info.tint;
				if (text)
					tween->m_originalTextColor = text->m_clr;
				tween->m_started = true;
			}
			double normalisedTime = tween->m_timeElapsed / duration;
			trans->m_pos = Tweening(tween->m_originalPos, target, normalisedTime);
			trans->m_scale = Tweening(tween->m_originalScale, scale, normalisedTime);
			trans->m_rot = Tweening(tween->m_originalRot, rot, normalisedTime);
			if (sprite) // set the sprite color if sprite component exists
			{
				sprite->m_spriteData.SetTint(
					Tweening(tween->m_originalSpriteColor, spriteColor, normalisedTime));
			}
			if (text) // set the text color if text component exists
			{
				text->SetColor(Tweening(tween->m_originalTextColor, textColor, normalisedTime));
			}
		}
		tween->m_timeElapsed += dt;
	}
	frc.EndSystemTimer("Tween System");
}

vec3 TweenSystem::Tweening(vec3 start, vec3 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}

Colorf GE::Systems::TweenSystem::Tweening(Colorf start, Colorf end, double normalisedTime)
{
	return start + (static_cast<GLfloat>(normalisedTime) * (end - start));
}
