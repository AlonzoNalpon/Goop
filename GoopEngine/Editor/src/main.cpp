//#include <GLFW/glfw3.h>
#include <pch.h>
#include <iostream>
#include <Window/Window.h>


//#define EXCEPTION_TEST
#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

#ifdef ASSET_M_TEST
#include "../AssetManager/AssetManager.h"
#endif //ASSET_M_TEST

#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include <FrameRateController/FrameRateController.h>
#include "../AssetManager/AssetManager.h"
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>
#endif

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#ifdef GRAPHICS_TEST
  
  WindowSystem::Window window{ 800, 800, "GOOP"}; // WINDOW
  window.CreateAppWindow();             // create the window
  GE::FPS::FrameRateController& fRC{ GE::FPS::FrameRateController::GetInstance() };
  Graphics::GraphicsEngine& gEngine{Graphics::GraphicsEngine::GetInstance()};     // my graphics engine
  

  fRC.InitFrameRateController(60);
  // Now we get the asset manager
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadDeserializedData(); // load the images we need
  am->LoadImageW(ASSETS_PATH + "MineWorm.png");

  gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color

  while (!window.GetWindowShouldClose())
  {
    fRC.StartFrame();
    window.SetWindowTitle((std::string{"GOOP ENGINE | FPS: "} + std::to_string(fRC.GetFPS())).c_str()); // this is how you set window title
    gEngine.Draw();
    Graphics::GraphicsEngine::DrawLine({ 0,0 }, { 300, 0 }, { 1, 0, 0 });
    window.SwapBuffers();
    fRC.EndFrame();
  }

  am->FreeImages(); // cleanup the images
#endif
#ifdef ECS_TEST
  try
  {
    Scene scn;
    scn.Start();
    scn.Update();
    scn.Exit();
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
  }
#endif // ECS_TEST

#ifdef ASSET_M_TEST
  //AssetManager::LoadImage();
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  //am->LoadImage("Assets/VADIM.jpg");
  //am->LoadDirectory("Assets/");
  //am->GetName(3);
  //am->GetID(am->GetName(3));
  am->LoadDeserializedData();
  am->FreeImage("Assets/Knight.png");
  am->FreeImage("Assets/Green Girl.png");
  am->FreeImages();
#endif

  
  return 0;
}