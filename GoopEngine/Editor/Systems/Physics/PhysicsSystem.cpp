#include <Physics/PhysicsSystem.h>
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

		//check if forces' lifetime is up & remove it
		for (LinearForce itr : vel->m_forces) {
			
		}

		for (LinearForce itr : vel->m_forces) {
			vel->m_sumMagnitude += itr.m_magnitude;
		}

		/*vel->m_vel += dt * (updateVel->m_acc + getGravity->m_gravity);
		updatePos->m_pos += dt * updateVel->m_vel;*/
	}
}