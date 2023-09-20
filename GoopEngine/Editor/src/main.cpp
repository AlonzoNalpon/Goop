#include <pch.h>

#include <iostream>
#include <Window/Window.h>

//#define EXCEPTION_TEST
#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

#include <FrameRateController/FrameRateController.h>

#define ASSET_M_TEST
#ifdef ASSET_M_TEST
#include "../AssetManager/AssetManager.h"
#endif //ASSET_M_TEST

#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include "../AssetManager/AssetManager.h"
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>
#endif

#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>

#define SERIALIZE_TEST
#ifdef SERIALIZE_TEST
#include <iomanip>
#include "../Serialization/AssetGooStream.h"
#include "../Serialization/SpriteGooStream.h"
#ifdef _DEBUG
// << overload for printing to ostream
std::ostream& operator<<(std::ostream& os, GE::Serialization::SpriteData const& sprite)
{
  os << "Name: " << sprite.m_id << "\nFile: " << sprite.m_filePath
    << "\nSlices: " << sprite.m_slices << "\nStacks: " << sprite.m_stacks
    << "\nFrames: " << sprite.m_frames;
  return os;
}
#endif
#endif // SERIALIZE_TEST

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef GRAPHICS_TEST
  
  GE::FPS::FrameRateController& fRC{ GE::FPS::FrameRateController::GetInstance() };
  Graphics::GraphicsEngine& gEngine{Graphics::GraphicsEngine::GetInstance()};     // my graphics engine
  

  fRC.InitFrameRateController(60);
  // Now we get the asset manager
#ifdef ASSET_M_TEST
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadJSONData("../Assets/AssetsToLoadTest/Images.json", GE::AssetManager::IMAGES);
  am->LoadJSONData("../Assets/AssetsToLoadTest/Config.json", GE::AssetManager::CONFIG);
  am->LoadJSONData("../Assets/AssetsToLoadTest/Sprites.txt", GE::AssetManager::ANIMATION);
  //am->SpriteCheck();
#endif

#ifdef GRAPHICS_TEST
  WindowSystem::Window window{ am->GetConfigData("Window Width"), am->GetConfigData("Window Height"), "GOOP"};
  window.CreateAppWindow();
  window.SetWindowTitle(am->GetConfigData("Window Title", 0)); // this is how you set window title
  // Now we get the asset manager
  am->LoadDeserializedData(); // load the images we need
  am->LoadImageW(ASSETS_PATH + "MineWorm.png");

  gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color

  am->FreeImages(); // cleanup the images
#endif

#ifdef SERIALIZE_TEST
  GE::Serialization::SpriteGooStream::container_type assets;
  std::string const fileName{ "../Assets/AssetsToLoadTest/sprites.txt" };
  GE::Serialization::SpriteGooStream sgs{ fileName };
  if (!sgs)
  {
    std::cout << "Error deserializing " << fileName << "\n";
  }
  if (!sgs.Unload(assets))
  {
    std::cout << "Error unloading assets into container" << "\n";
  }

  std::cout << "Deserialized " << fileName << ":\n";
  for (auto const& entry : assets)
  {
    std::cout << entry << "\n";
  }
#endif

#ifdef ECS_TEST
  Scene scn;
  try
  {
    scn.Start();
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
  }
#endif // ECS_TEST

  while (!window.GetWindowShouldClose())
  {
    fRC.StartFrame();
    window.SetWindowTitle((std::string{"GOOP ENGINE | FPS: "} + std::to_string(fRC.GetFPS())).c_str()); // this is how you set window title
#ifdef ECS_TEST
    scn.Update();
#endif // ECS_TEST
    gEngine.Draw();
    Graphics::GraphicsEngine::DrawLine({ 0,0 }, { 300, 0 }, { 1, 0, 0 }); // THIS IS HOW YOU DRAW A LINE (no need for calling getinstance)
    window.SwapBuffers();
    fRC.EndFrame();
  }
#endif

#ifdef ECS_TEST
  scn.Exit();
#endif // ECS_TEST


  return 1;
}