#include <pch.h>
#include <GameStateManager/GameStateManager.h>
using namespace GE::GSM;

void GE::GSM::GameStateManager::SetNextScene(std::string next_scene)
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
		SetNextScene("Start");
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

