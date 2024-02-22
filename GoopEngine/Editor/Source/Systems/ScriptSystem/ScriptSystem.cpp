/*!*********************************************************************
\file   ScriptSystem.cpp
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Updates all updatable script on the entities

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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

	for (Entity const& entity : GetUpdatableEntities())
	{
		Scripts* scriptHan = m_ecs->GetComponent<Scripts>(entity);
		if (scriptHan != nullptr)
		{
			scriptHan->UpdateAllScripts();
		}
	}
	
	frc.EndSystemTimer("Script System");
}

