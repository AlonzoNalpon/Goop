#include <pch.h>
#include "Physics.h"
#include "Gravity.h"
#include "Transform.h"
#include "Velocity.h"

using namespace GE::Physics;
using namespace GE::ECS;

void PhysSys::Awake()
{
	m_ecs = &EntityComponentSystem::GetInstance();
}

void PhysSys::Update()
{
	//update func passes curr entity
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (Entity entity : m_entities) {
		Velocity* updateVel = m_ecs->GetComponent<Velocity>(entity);
		Gravity* getGravity = m_ecs->GetComponent<Gravity>(entity);
		updateVel->m_vel += dt * (updateVel->m_acc + getGravity->m_gravity);
		Transform* updatePos = m_ecs->GetComponent<Transform>(entity);
		updatePos->m_pos += dt * updateVel->m_vel;
	}
}