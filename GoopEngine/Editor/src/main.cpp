#include <pch.h>

//#include <GLFW/glfw3.h>
#include <iostream>
#include <GLApp/Window/Window.h>
//#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include <GLApp/Graphics/GraphicsEngine.h>
#endif
//#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

#define ASSET_M_TEST
#ifdef ASSET_M_TEST
#include "../AssetManager/AssetManager.h"
#endif //ASSET_M_TEST

#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include <GLApp/Window/Window.h>
#include <GLApp/Graphics/GraphicsEngine.h>
#endif

//#define SERIALIZE_TEST
#ifdef SERIALIZE_TEST
#include <iomanip>
#include "../Serialization/AssetGooStream.h"
#endif

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  //if (!glfwInit())
  //{
  //  // Initialization failed!
  //  std::cout << "Failed to initialize GLFW! Exiting now..." << std::endl;
  //  std::exit(EXIT_FAILURE);
  //}

  //Goop::Window window(100, 100, L"WHAT");
  //window.CreateOGLWindow();
#ifdef ASSET_M_TEST
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadJSONData("../Assets/AssetsToLoadTest/Images.json", GE::AssetManager::IMAGES);
  am->LoadJSONData("../Assets/AssetsToLoadTest/Config.json", GE::AssetManager::CONFIG);
#endif

#ifdef GRAPHICS_TEST
  WindowSystem::Window window{ am->GetConfigData("Window Width"), am->GetConfigData("Window Height"), "GOOP"};
  window.CreateAppWindow();
  Graphics::GraphicsEngine gEngine;
  
  gEngine.Init(Graphics::Colorf{});

  while (!window.GetWindowShouldClose()) {
    gEngine.Draw();
    window.SwapBuffers();
  }
#endif
 
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

#ifdef ECS_TEST
  Scene scn;
  scn.Start();
  scn.Update();
  scn.Exit();
#endif // ECS_TEST
  return 1;
}