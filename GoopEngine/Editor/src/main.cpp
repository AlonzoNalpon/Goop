#include <pch.h>

#include <iostream>
#include <Window/Window.h>

//#define EXCEPTION_TEST
#include "../ECS/ECS Example/Scene.h"

#include <FrameRateController/FrameRateController.h>

#include "../AssetManager/AssetManager.h"

#include "../AssetManager/AssetManager.h"
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>


#include <Physics/PhysicsSystem.h>
#include <Physics/CollisionSystem.h>

#define SERIALIZE_TEST
#ifdef SERIALIZE_TEST
#include <iomanip>
#include "../Serialization/AssetGooStream.h"
#include "../Serialization/SpriteGooStream.h"
#include "../Serialization/PrefabGooStream.h"
#include "../ObjectFactory/ObjectFactory.h"
#endif
#define MEMORY_TEST
#ifdef MEMORY_TEST
#include "../MemoryManager/MemoryManager.h"
#endif
#include "../EditorUI/ImGuiUI.h"
#include <time.h>

int main(int /*argc*/, char* /*argv*/[])
{
  srand(static_cast<unsigned int>(time(NULL)));
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // INIT FUNCTIONS
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadJSONData("Assets/Data/Images.json", GE::AssetManager::IMAGES);
  am->LoadJSONData("Assets/Data/Config.json", GE::AssetManager::CONFIG);
  am->LoadJSONData("Assets/Data/Sprites.txt", GE::AssetManager::ANIMATION);

  GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };
  of.LoadPrefabsFromFile();
  of.RegisterComponentsAndSystems();
  GE::Events::EventManager::GetInstance().SubscribeAllListeners();

  GE::FPS::FrameRateController& fRC{ GE::FPS::FrameRateController::GetInstance() };
  Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };     // my graphics engine
  fRC.InitFrameRateController(am->GetConfigData<int>("FPS Limit").value(), am->GetConfigData<int>("FPS Check Interval").value());


#ifdef MEMORY_TEST
  GE::Memory::MemoryManager* memMan{ &(GE::Memory::MemoryManager::GetInstance()) };
  memMan->InitializeAllAlocators(am->GetConfigData<int>("Memory Size").value());
#endif

  WindowSystem::Window window{ am->GetConfigData<int>("Window Width").value(), am->GetConfigData<int>("Window Height").value(), "GOOP"};
  window.CreateAppWindow();
  //window.SetWindowTitle(am->GetConfigData<const char*>("Window Title").value()); // this is how you set window title

  GE::EditorGUI::ImGuiUI imgui;
  imgui.Init(window);

  // Now we get the asset manager
  am->LoadDeserializedData(); // load the images we need
  am->LoadImageW(ASSETS_PATH + "MineWorm.png");
  gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
  am->FreeImages(); // cleanup the images


  GE::Input::InputManager* im = &(GE::Input::InputManager::GetInstance());
  im->InitInputManager(window.GetWindow(), am->GetConfigData<int>("Window Width").value(), am->GetConfigData<int>("Window Height").value(), 0.1);
  GE::FPS::FrameRateController* fps_control = &(GE::FPS::FrameRateController::GetInstance());
  fps_control->InitFrameRateController(60, 1);

#ifdef SERIALIZE_TEST
  GE::ObjectFactory::ObjectFactory::ObjectFactoryTest();

  of.ObjectJsonLoader("Assets/Data/Scene.json");
  of.JoelTest();
  GE::Serialization::PrefabGooStream::PrefabLoadTest();
#endif
  GE::Debug::ErrorLogger::GetInstance().SuppressLogMessages(true);

#ifdef MEMORY_TEST
  //memMan->TestAllAllocators();
#endif

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

  while (!window.GetWindowShouldClose())
  {
    fRC.StartFrame();

    fRC.StartSystemTimer();
    im->UpdateInput();
    fRC.EndSystemTimer("Input System");

    static bool renderUI = false;
    if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_G))
    {
      renderUI = !renderUI;
    }

    if (renderUI)
    {
      fRC.StartSystemTimer();
      imgui.Update();
      fRC.EndSystemTimer("ImGui Update");
    }
    gEngine.ClearBuffer();

    fRC.StartSystemTimer();
    scn.Update();
    fRC.EndSystemTimer("Scene Update");

    fRC.StartSystemTimer();
    gEngine.Draw();
    fRC.EndSystemTimer("Draw");

    if (renderUI)
    {
      fRC.StartSystemTimer();
      imgui.Render();
      fRC.EndSystemTimer("ImGui Render");
    }

    std::stringstream ss;
    ss << GE::AssetManager::AssetManager::GetInstance().GetConfigData<std::string>("Window Title").value() << " | FPS: " << std::fixed << std::setfill('0') << std::setw(5) << std::setprecision(2) << fRC.GetFPS();
    for (auto system : fRC.GetSystemTimers())
    {
      ss << " | " << system.first << ": " << std::setw(4) << system.second.count() << "us";
    }
    window.SetWindowTitle(ss.str().c_str()); // this is how you set window title

    window.SwapBuffers();
    fRC.EndFrame();
  }

  scn.Exit();

  imgui.Exit();

  return 1;
}