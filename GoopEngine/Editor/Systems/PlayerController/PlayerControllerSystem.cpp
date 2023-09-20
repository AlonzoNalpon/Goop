#include <PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>
#include <Component/Transform.h>

using vec2 = GE::Math::dVec2;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void PlayerControllerSystem::Awake() 
{
	m_ecs = &EntityComponentSystem::GetInstance();
}

void PlayerControllerSystem::Update() 
{
	for (Entity entity : m_entities) {

		Tween* tween = m_ecs->GetComponent<Tween>(entity);

		if (tween->m_tweens.size() == 0)
		{
			continue;
		}

		Transform* trans = m_ecs->GetComponent<Transform>(entity);
		vec2& targetPos = m_ecs->GetComponent<Tween>(entity)->m_tweens.front();

		if (tween->m_timeElapsed > tween->m_timePerTween) 
		{
			tween->m_timeElapsed -= tween->m_timePerTween;
			tween->m_originalPos = targetPos;
			double normalisedTime = tween->m_timeElapsed / tween->m_timePerTween;
			trans->m_pos = Tweening(tween->m_originalPos, targetPos, normalisedTime);
			tween->m_tweens.pop();
			std::cout << "pop\n";
		}
		else 
		{
			if (!tween->m_started)
			{
				tween->m_originalPos = trans->m_pos;
				tween->m_started = true;
			}
			double normalisedTime = tween->m_timeElapsed / tween->m_timePerTween;
			trans->m_pos = Tweening(tween->m_originalPos, targetPos, normalisedTime);
		}
		tween->m_timeElapsed += GE::FPS::FrameRateController::GetInstance().GetDeltaTime();

		std::cout << "Player Position: [" << trans->m_pos.x << ", " << trans->m_pos.y << "]\n";
	}
}

vec2 PlayerControllerSystem::Tweening(vec2 start, vec2 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}