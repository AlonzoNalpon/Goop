#include <PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>
#include <Component/Transform.h>
#include <math.h>
#include <Events/InputEvents.h>
#include <Events/EventManager.h>

using vec2 = GE::Math::dVec2;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;
constexpr double pi = 3.14159265358979323846;
void PlayerControllerSystem::Awake() 
{
	m_ecs = &EntityComponentSystem::GetInstance();
	Events::EventManager::GetInstance().Subscribe<Events::KeyHeldEvent>(this);
	Events::EventManager::GetInstance().Subscribe<Events::KeyTriggeredEvent>(this);
}

void PlayerControllerSystem::Update() 
{
	auto& inputMan{ Input::InputManager::GetInstance() };
	if (!(inputMan.IsKeyHeld(GPK_SPACE) || inputMan.IsKeyTriggered(GPK_SPACE)))
		return;

	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();

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
			//std::cout << "pop\n";
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
		tween->m_timeElapsed += dt;

		trans->m_rot = fmod(trans->m_rot + dt, pi * 2.0); // ROTATING PLAYER
		//std::cout << "Player Position: [" << trans->m_pos.x << ", " << trans->m_pos.y << "]\n";
	}
}

void PlayerControllerSystem::HandleEvent(Events::Event const* event)
{
	if (event->GetCategory() == Events::EVENT_TYPE::KEY_HELD)
	{
		KEY_CODE const key{ static_cast<Events::KeyHeldEvent const*>(event)->GetKey() };
		if (key == GPK_H)
		{
			
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
		else if (key == GPK_J)
		{

			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
	else if (event->GetCategory() == Events::EVENT_TYPE::KEY_TRIGGERED)
	{
		KEY_CODE const key{ static_cast<Events::KeyHeldEvent const*>(event)->GetKey() };
		if (key == GPK_K)
		{
			
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
}

vec2 PlayerControllerSystem::Tweening(vec2 start, vec2 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}