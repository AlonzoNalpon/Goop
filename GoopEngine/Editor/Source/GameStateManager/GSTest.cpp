#include <pch.h>
#include <GameStateManager/GSTest.h>
#include <AssetManager/AssetManager.h>

void GameStateTestLoad(void)
{
  GE::Assets::AssetManager *am = &GE::Assets::AssetManager::GetInstance();
  am->LoadJSONData("./Assets/Data/Scene/TestScene.json", GE::Assets::SCENE);

}

void GameStateTestInit(void)
{

}

void GameStateTestUpdate(void)
{

}

void GameStateTestDraw(void)
{

}

void GameStateTestFree(void)
{

}

void GameStateTestUnload(void)
{

}
