#include <pch.h>
#include <GameStateManager/GameStateManager.h>
using namespace GE::GSM;

GE::GSM::GameStateManager::GameStateManager()
{
}

void GE::GSM::GameStateManager::SetNextScene(std::string next_scene)
{
	sm.SetNextScene(next_scene);
}

void GameStateManager::Init()
{
	sm.Init();
	sm.LoadScene();
	sm.InitScene();
}

void GameStateManager::Update()
{
	if (sm.GetNextScene() != sm.GetCurrentScene())
	{
		std::cout << "Change Scene Detected: " << sm.GetNextScene() << " | " << sm.GetCurrentScene() << std::endl;
		//this->SetNextScene(sm.GetNextScene());
		//sm.LoadScene();
		//sm.InitScene();

	}
}

void GE::GSM::GameStateManager::Exit()
{
	// Example of removing systems
}


