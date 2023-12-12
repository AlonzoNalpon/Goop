#include <pch.h>
#include "ScriptSystem.h"
#include <Events/EventManager.h>
#include <Component/Scripts.h>

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

void ScriptSystem::FixedUpdate() 
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	std::set<ECS::Entity> ents = GetUpdatableEntities();

	for (Entity entity : GetUpdatableEntities()) {

		Scripts* scriptHan = m_ecs->GetComponent<Scripts>(entity);
		if (scriptHan != nullptr)
		{
			scriptHan->UpdateAllScripts(entity);
		}
	}
	
	frc.EndSystemTimer("Script System");
}

