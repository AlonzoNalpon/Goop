/*!*********************************************************************
\file   GameStateManager.cpp
\author loh.j@digipen.edu
\date   24-October-2023
\brief
	Currently controls the scenes but will include gamestates in the 
	future.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <GameStateManager/GameStateManager.h>
using namespace GE::GSM;

void GE::GSM::GameStateManager::SetNextScene(std::string next_scene)
{
	auto& scenes =  GE::Assets::AssetManager::GetInstance().GetScenes();
	if (scenes.find(next_scene) == scenes.end())
	{
		GE::Debug::ErrorLogger::GetInstance().LogWarning("Scene " + next_scene + " does not exist. Unable to load");
		return;
	}

	try
	{
		if (sm.GetCurrentScene() == next_scene)
		{
			sm.RestartScene();
		}
		else
		{
			sm.SetNextScene(next_scene);
		}
	}
	catch (GE::Debug::IExceptionBase& e)
	{
		e.LogSource();
	}
}

std::string GE::GSM::GameStateManager::GetCurrentScene() const
{
	return sm.GetCurrentScene();
}

void GE::GSM::GameStateManager::SaveScene() const
{
	sm.SaveScene();
}

void GE::GSM::GameStateManager::Restart()
{
	sm.RestartScene();
}

void GE::GSM::GameStateManager::LoadSceneFromExplorer(std::string const& filepath)
{
	sm.LoadSceneFromExplorer(filepath);
}

void GameStateManager::Init()
{
	sm.Init();
	sm.LoadScene();
	sm.InitScene();
}

void GameStateManager::Update()
{
	if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_RIGHT))
	{
		SetNextScene("SceneTest");
	}
	if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_LEFT))
	{
		SetNextScene("Robot");
	}
	if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_UP))
	{
		Restart();
	}
}

void GE::GSM::GameStateManager::Exit()
{
	// Example of removing systems
}
