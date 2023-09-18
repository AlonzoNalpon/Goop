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

#include "../AssetManager/AssetManager.h"
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>
#endif

#define SERIALIZE_TEST
#ifdef SERIALIZE_TEST
#include <iomanip>
#include "../Serialization/AssetGooStream.h"
#endif // SERIALIZE_TEST

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#ifdef GRAPHICS_TEST
  WindowSystem::Window window{ 800, 800, "GOOP"};
  window.CreateAppWindow();
  Graphics::GraphicsEngine gEngine;     
  window.SetWindowTitle("GOOP ENGINE"); // this is how you set window title
  // Now we get the asset manager
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadDeserializedData(); // load the images we need

  gEngine.Init(Graphics::Colorf{ .4f }); // Initialize the engine with this clear color

  while (!window.GetWindowShouldClose()) {
    gEngine.Draw();
    window.SwapBuffers();
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

#ifdef SERIALIZE_TEST
  std::map<std::string, std::string> assets;
  std::string const fileName{ "../Assets/AssetsToLoadTest/bat_file_output.json" };
  GE::Serialization::AssetGooStream ags{ fileName };
  if (!ags)
  {
    std::cout << "Error deserializing " << fileName << "\n";
  }
  if (!ags.Unload(assets))
  {
    std::cout << "Error unloading assets into container" << "\n";
}

  std::cout << "Deserialized " << fileName << ":\n";
  for (std::pair<std::string, std::string> const& entry : assets)
  {
    std::cout << "Key: " << std::left << std::setw(10) << entry.first << " | Value: " << entry.second << "\n";
  }
#endif

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