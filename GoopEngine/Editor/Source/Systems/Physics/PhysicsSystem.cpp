#include <pch.h>
#include <Systems/Physics/PhysicsSystem.h>
#include <Component/Transform.h>
#include <Component/Velocity.h>
#include <numeric>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void PhysicsSystem::FixedUpdate()
{
	/*auto& inputMan{ Input::InputManager::GetInstance() };
	if (!(inputMan.IsKeyHeld(GPK_SPACE) || inputMan.IsKeyTriggered(GPK_SPACE)))
		return;*/

	//update func passes curr entity
	double dt = GE::FPS::FrameRateController::GetInstance().GetFixedDeltaTime();
	for (Entity entity : GetUpdatableEntities())
	{
		Velocity* vel = m_ecs->GetComponent<Velocity>(entity);
		Transform* pos = m_ecs->GetComponent<Transform>(entity);

		if (vel == NULL)
		{
			continue;
		}

		vel->m_sumMagnitude = {};

		//removing time from lifetime till <= 0 & removing it when lifetime reaches <= 0
		for (auto itr{ vel->m_forces.begin() }; itr != vel->m_forces.end();)
		{
			if (itr->m_isActive)
			{
				itr->m_age += dt;

				vel->m_sumMagnitude += itr->m_magnitude * itr->m_age;
				if (itr->m_age >= itr->m_lifetime)
				{
					itr->m_isActive = false;
					itr = vel->m_forces.erase(itr);
					continue;
				}
			}

			++itr;
		}

		if (vel->m_forces.empty())
		{
			vel->m_sumMagnitude = {};
			vel->m_acc = {};
		}

		if (vel->m_mass == 0)
		{
			std::string message = "Dividing by 0: " + m_ecs->GetEntityName(entity) + " has mass of 0";
			throw Debug::Exception<PhysicsSystem>(Debug::LEVEL_ERROR, ErrMsg(message));
		}

		vel->m_acc += vel->m_sumMagnitude * (1 / vel->m_mass);
		vel->m_vel += dt * vel->m_acc;

		vel->m_vel *= vel->m_dragForce.m_magnitude;
		
		if (vel->GetMagnitude(vel->m_vel) <= 0.01 && vel->GetMagnitude(vel->m_vel) >= -0.01)
		{
			vel->m_vel = {};
		}

		pos->m_pos += dt * vel->m_vel;
	}
}