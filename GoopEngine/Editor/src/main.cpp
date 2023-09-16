//#include <GLFW/glfw3.h>
#include <iostream>
#include <GLApp/Window/Window.h>
#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include <GLApp/Graphics/GraphicsEngine.h>
#include "../AssetManager/AssetManager.h"
#endif

#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

#ifdef ASSET_M_TEST
#include "../AssetManager/AssetManager.h"
#endif //ASSET_M_TEST


int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef ECS_TEST
  Scene scn;
  scn.Start();
  scn.Update();
  scn.Exit();
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

#ifdef GRAPHICS_TEST
  windowSystem::Window window{ 800, 800, "GOOP" };
  window.CreateWindow();
  Graphics::GraphicsEngine gEngine;

  // Now we get the asset manager
  GE::AssetManager::AssetManager* am = &GE::AssetManager::AssetManager::GetInstance();
  am->LoadDeserializedData(); // load the images we need

  //std::cout << "THE SIZE IS " << sizeof(unsigned long long) << std::endl;

  gEngine.Init(Graphics::Colorf{}); // init the clear color to be BLACK
  while (!window.GetWindowShouldClose()) {
    gEngine.Draw();
    // WE ARE NOT POLLING FOR ANYTHING AT THE MOMENT
    window.SwapBuffers();
  }
  am->FreeImages(); // cleanup the images
#endif
  
  return 0;
}