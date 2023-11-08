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

	for (Entity entity : m_entities) 
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
		auto [target, duration] = m_ecs->GetComponent<Tween>(entity)->m_tweens.front();

		if (tween->m_timeElapsed > duration)
		{
			tween->m_timeElapsed -= duration;
			tween->m_originalPos = target;
			double normalisedTime = tween->m_timeElapsed / duration;
			trans->m_pos = Tweening(tween->m_originalPos, target, normalisedTime);
			tween->m_tweens.pop_front();
		}
		else
		{
			if (!tween->m_started)
			{
				tween->m_originalPos = trans->m_pos;
				tween->m_started = true;
			}
			double normalisedTime = tween->m_timeElapsed / duration;
			trans->m_pos = Tweening(tween->m_originalPos, target, normalisedTime);
		}
		tween->m_timeElapsed += dt;
	}
	frc.EndSystemTimer("Tween System");
}

vec3 TweenSystem::Tweening(vec3 start, vec3 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}
