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
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
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
		for (auto itr{ vel->m_linearForces.begin() }; itr != vel->m_linearForces.end();)
		{
			if (itr->m_isActive)
			{
				itr->m_age += dt;

				vel->m_sumMagnitude += itr->m_magnitude * dt;
				if (itr->m_age >= itr->m_lifetime)
				{
					itr->m_isActive = false;
					itr = vel->m_linearForces.erase(itr);
					continue;
				}
			}

			vel->m_sumMagnitude += vel->m_mass * vel->m_gravity * dt;

			++itr;
		}

		if (vel->m_linearForces.empty())
		{
			//gravity
			vel->m_sumMagnitude += vel->m_mass * vel->m_gravity;
		}

		if (vel->m_mass == 0)
		{
			std::string message = "Dividing by 0: " + m_ecs->GetEntityName(entity) + " has mass of 0";
			throw Debug::Exception<PhysicsSystem>(Debug::LEVEL_ERROR, ErrMsg(message));
		}

		vel->m_acc += vel->m_sumMagnitude * vel->m_mass;
		vel->m_vel += dt * vel->m_acc;

		////impulse force
		//vec3 impulse{};
		//for (auto itr{ vel->m_impulseForces.begin() }; itr != vel->m_impulseForces.end();)
		//{
		//	impulse += itr->m_magnitude * itr->m_duration;
		//	++itr;
		//}
		//vel->m_vel += impulse / vel->m_mass;
		//vel->m_impulseForces.clear();

		//drag force
		vel->m_vel *= 1 - vel->m_dragForce.m_magnitude;
		
		if (vel->GetMagnitude(vel->m_vel) <= 0.01 && vel->GetMagnitude(vel->m_vel) >= -0.01)
		{
			vel->m_vel = {};
		}

		pos->m_pos += dt * vel->m_vel;
	}
	frc.EndSystemTimer("Physics");
}