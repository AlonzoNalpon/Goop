#include <Systems/Physics/PhysicsSystem.h>
#include <Component/Transform.h>
#include <Component/Velocity.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void PhysicsSystem::Update()
{
	auto& inputMan{ Input::InputManager::GetInstance() };
	if (!(inputMan.IsKeyHeld(GPK_SPACE) || inputMan.IsKeyTriggered(GPK_SPACE)))
		return;
	//update func passes curr entity
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (Entity entity : GetUpdatableEntities()) {
		//testing acceleration
		Velocity* vel = m_ecs->GetComponent<Velocity>(entity);
		Transform* pos = m_ecs->GetComponent<Transform>(entity);

		if (vel == NULL)
		{
			continue;
		}

		//removing time from lifetime till <= 0 & removing it when lifetime reaches <= 0
		for (LinearForce itr : vel->m_forces) {
			if (itr.m_lifetime <= 0) {
				itr.m_isActive = false;
			}
			else
			{
				itr.m_lifetime -= dt;
			}

			if (itr.m_isActive)
			{
				vel->m_sumMagnitude += itr.m_magnitude;
			}
		}

		if (vel->m_dragForce.m_isActive)
		{
			vel->m_sumMagnitude += vel->m_dragForce.m_magnitude;
		}

		vel->m_acc = vel->m_sumMagnitude * (1 / vel->m_mass);
		vel->m_vel += dt * vel->m_acc;
		pos->m_pos += dt * vel->m_vel;

		/*vel->m_vel += dt * (updateVel->m_acc + getGravity->m_gravity);
		updatePos->m_pos += dt * updateVel->m_vel;*/
	}
}