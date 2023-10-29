#include <pch.h>
#include <GameStateManager/GameStateManager.h>
using namespace GE::GSM;

void GE::GSM::GameStateManager::SetNextScene(std::string next_scene)
{
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


