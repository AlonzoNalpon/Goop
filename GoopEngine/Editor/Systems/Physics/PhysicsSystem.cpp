#include <pch.h>
#include <Physics/PhysicsSystem.h>
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Gravity.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void PhysicsSystem::Update()
{
	//update func passes curr entity
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (Entity entity : m_entities) {
		//testing acceleration
		Velocity* updateVel = m_ecs->GetComponent<Velocity>(entity);
		Transform* updatePos = m_ecs->GetComponent<Transform>(entity);
		std::cout << "Entity's curr pos: " << updatePos->m_pos.x << ',' << updatePos->m_pos.y << std::endl;
		updateVel->m_acc = { 1, 2 };

		Gravity* getGravity = m_ecs->GetComponent<Gravity>(entity);
		updateVel->m_vel += dt * (updateVel->m_acc + getGravity->m_gravity);
		updatePos->m_pos += dt * updateVel->m_vel;
		std::cout << "Entity's new pos: " << updatePos->m_pos.x << ',' << updatePos->m_pos.y << std::endl;
	}
}