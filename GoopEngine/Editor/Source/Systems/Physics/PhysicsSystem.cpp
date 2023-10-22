#include <pch.h>
#include <Systems/Physics/PhysicsSystem.h>
#include <Component/Transform.h>
#include <Component/Velocity.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void PhysicsSystem::FixedUpdate()
{
	auto& inputMan{ Input::InputManager::GetInstance() };
	/*if (!(inputMan.IsKeyHeld(GPK_SPACE) || inputMan.IsKeyTriggered(GPK_SPACE)))
		return;*/

	//update func passes curr entity
	double dt = GE::FPS::FrameRateController::GetInstance().GetFixedDeltaTime();
	for (Entity entity : GetUpdatableEntities()) {
		Velocity* vel = m_ecs->GetComponent<Velocity>(entity);
		Transform* pos = m_ecs->GetComponent<Transform>(entity);

		if (vel == NULL)
		{
			continue;
		}

		if (inputMan.IsKeyTriggered(GPK_COMMA))
		{
			vel->AddForce({ 1,0,0 }, 2);
		}
		if (inputMan.IsKeyTriggered(GPK_PERIOD))
		{
			vel->AddForce({ -5,0,0 }, 1);
		}
		if (inputMan.IsKeyTriggered(GPK_SEMICOLON))
		{
			vel->ActivateDrag(false);
		}
		if (inputMan.IsKeyTriggered(GPK_APOSTROPHE))
		{
			vel->ActivateDrag(true);
		}

		//removing time from lifetime till <= 0 & removing it when lifetime reaches <= 0
		for (size_t i{}; i < vel->m_forces.size(); ++i) 
		{
			LinearForce& itr = vel->m_forces[i];
			if (itr.m_isActive)
			{
				vel->m_sumMagnitude += itr.m_magnitude;
				itr.m_lifetime -= dt;

				//remove age -> use lifetime only
				if (itr.m_lifetime <= 0) {
					itr.m_isActive = false;
					vel->m_forces.erase(vel->m_forces.begin() + i);
				}
			}
		}

		if (vel->m_dragForce.m_isActive)
		{
			vel->m_sumMagnitude += vel->m_dragForce.m_magnitude;
		}

		if (!(vel->m_dragForce.m_isActive) && vel->m_forces.empty())
		{
			vel->m_sumMagnitude = {};
			vel->m_vel = {};
		}

		if (vel->m_mass == 0) {
			std::string message = "Dividing by 0: " + m_ecs->GetEntityName(entity) + " has mass of 0";
			throw Debug::Exception<PhysicsSystem>(Debug::LEVEL_ERROR, ErrMsg(message));
		}

		vel->m_acc = vel->m_sumMagnitude * (1 / vel->m_mass);
		vel->m_vel += dt * vel->m_acc;
		pos->m_pos += dt * vel->m_vel;
	}
}