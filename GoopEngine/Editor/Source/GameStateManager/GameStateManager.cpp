#include <pch.h>
#include <GameStateManager/GameStateManager.h>
using namespace GE::GSM;

GE::GSM::GameStateManager::GameStateManager()
{
	currScene = "SceneTest";
	nextScene = "SceneTest";
}

void GameStateManager::Init()
{
	sm.Init();
	Update();
}

void GameStateManager::Update()
{
	sm.SetNextScene(nextScene);
	sm.LoadScene();
	sm.InitScene();
}

void GE::GSM::GameStateManager::Exit()
{
	// Example of removing systems
}


