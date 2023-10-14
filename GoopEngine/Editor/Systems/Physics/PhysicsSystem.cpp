#include <Physics/PhysicsSystem.h>
#include <Component/Transform.h>
#include <Component/Force.h>

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
		Velocity* updateVel = m_ecs->GetComponent<Velocity>(entity);
		Transform* updatePos = m_ecs->GetComponent<Transform>(entity);
		Gravity* getGravity = m_ecs->GetComponent<Gravity>(entity);

		if (updateVel == NULL || getGravity == NULL)
		{
			continue;
		}

		updateVel->m_vel += dt * (updateVel->m_acc + getGravity->m_gravity);
		updatePos->m_pos += dt * updateVel->m_vel;
	}
}