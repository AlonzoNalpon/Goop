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
#endif
#include "../EditorUI/ImGuiUI.h"

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  GE::FPS::FrameRateController& fRC{ GE::FPS::FrameRateController::GetInstance() };
  Graphics::GraphicsEngine& gEngine{Graphics::GraphicsEngine::GetInstance()};     // my graphics engine
  fRC.InitFrameRateController(60);

  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadJSONData("Assets/Data/Images.json", GE::AssetManager::IMAGES);
  am->LoadJSONData("Assets/Data/Config.json", GE::AssetManager::CONFIG);
  am->LoadJSONData("Assets/Data/Sprites.txt", GE::AssetManager::ANIMATION);

  WindowSystem::Window window{ am->GetConfigData("Window Width"), am->GetConfigData("Window Height"), "GOOP"};
  window.CreateAppWindow();
  window.SetWindowTitle(am->GetConfigData("Window Title", 0)); // this is how you set window title

  GE::EditorGUI::ImGuiUI imgui;
  imgui.Init(window);

  // Now we get the asset manager
  am->LoadDeserializedData(); // load the images we need
  am->LoadImageW(ASSETS_PATH + "MineWorm.png");
  gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
  am->FreeImages(); // cleanup the images


  GE::Input::InputManager* im = &(GE::Input::InputManager::GetInstance());
  im->InitInputManager(window.GetWindow(), am->GetConfigData("Window Width"), am->GetConfigData("Window Height"), 0.1);
  GE::FPS::FrameRateController* fps_control = &(GE::FPS::FrameRateController::GetInstance());
  fps_control->InitFrameRateController(60, 1);

  GE::ObjectFactory::ObjectFactory::GetInstance().LoadPrefabsFromFile();

#ifdef SERIALIZE_TEST
  GE::ObjectFactory::ObjectFactory::ObjectFactoryTest();

  GE::ObjectFactory::ObjectFactory& of = (GE::ObjectFactory::ObjectFactory::GetInstance());
  of.ObjectJsonLoader("Assets/Data/SERIALIZED.json");
  of.JoelTest();
  GE::Serialization::PrefabGooStream::PrefabLoadTest();
  //GE::Serialization::SpriteGooStream::container_type assets;
  //std::string const fileName{ "../Assets/AssetsToLoadTest/sprites.txt" };
  //GE::Serialization::SpriteGooStream sgs{ fileName };
  //if (!sgs)
  //{
  //  std::cout << "Error deserializing " << fileName << "\n";
  //}
  //if (!sgs.Unload(assets))
  //{
  //  std::cout << "Error unloading assets into container" << "\n";
  //}

  //std::cout << "Deserialized " << fileName << ":\n";
  //for (auto const& entry : assets)
  //{
  //  std::cout << entry << "\n";
  //}
#endif
  GE::Debug::ErrorLogger::GetInstance().SuppressLogMessages(true);

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

    im->UpdateInput();
    //im->TestInputManager();

    static bool renderUI = false;
    if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_G))
    {
      std::cout << "SDFSDFREUYGSDRYUFGREDGSDRGDRG\n";
      renderUI = !renderUI;
    }

    if (renderUI)
    {
      imgui.Update();
    }

    window.SetWindowTitle((std::string{"GOOP ENGINE | FPS: "} + std::to_string(fRC.GetFPS())).c_str()); // this is how you set window title
    gEngine.Draw();
    scn.Update();

    if (renderUI)
    {
      imgui.Render();
    }

    window.SwapBuffers();
    fRC.EndFrame();
  }

  scn.Exit();

  imgui.Exit();

  return 1;
}