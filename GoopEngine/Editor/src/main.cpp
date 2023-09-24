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
#endif // SERIALIZE_TEST

#define INPUT_TEST

#include "../EditorUI/ImGuiUI.h"

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

  GE::EditorGUI::ImGuiUI imgui;
  imgui.Init(window);

  // Now we get the asset manager
  am->LoadDeserializedData(); // load the images we need
  am->LoadImageW(ASSETS_PATH + "MineWorm.png");
  gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
  am->FreeImages(); // cleanup the images
#endif


#ifdef INPUT_TEST

  GE::Input::InputManager* im = &(GE::Input::InputManager::GetInstance());
  im->InitInputManager(window.GetWindow(),800,800);
#endif
  GE::FPS::FrameRateController* fps_control = &(GE::FPS::FrameRateController::GetInstance());
  fps_control->InitFrameRateController(60, 1);

#ifdef INPUT_TEST
  std::cout << "-------------------------------\n";
  std::cout << "To test Input Manager you can:\n 1.click/hold/release key A.\n 2.Click Mouse Left Button to print Mouse Position\n ";

#endif

#ifdef SERIALIZE_TEST
  GE::ObjectFactory::ObjectFactory::ObjectFactoryTest();

  GE::ObjectFactory::ObjectFactory& of = (GE::ObjectFactory::ObjectFactory::GetInstance());
  of.ObjectJsonLoader("../Assets/AssetsToLoadTest/SERIALIZED.json");
  of.JoelTest();

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

#ifdef INPUT_TEST
    im->UpdateInput();
    im->TestInputManager();
#endif

    imgui.Update();

    window.SetWindowTitle((std::string{"GOOP ENGINE | FPS: "} + std::to_string(fRC.GetFPS())).c_str()); // this is how you set window title
    gEngine.Draw();
#ifdef ECS_TEST
    scn.Update();
#endif // ECS_TEST

    imgui.Render();

    window.SwapBuffers();
    fRC.EndFrame();
  }
#endif

#ifdef ECS_TEST
  scn.Exit();
#endif // ECS_TEST

  imgui.Exit();

  return 1;
}