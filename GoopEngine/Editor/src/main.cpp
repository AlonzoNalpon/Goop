//#include <GLFW/glfw3.h>
#include <iostream>
#include "GLApp/Window/Window.h"

#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

#define ASSET_M_TEST
#ifdef ASSET_M_TEST
#include "../AssetManager/AssetManager.h"
void LoadImage()
{
}
#endif //ASSET_M_TEST


int main(int argc, char* argv[])
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
  windowSystem::Window window{ 640, 480, "GOOP"};
  window.CreateWindow();

#ifdef ECS_TEST
  Scene scn;
  scn.Start();
  scn.Update();
  scn.Exit();
#endif // ECS_TEST

#ifdef ASSET_M_TEST
  //Asset_M::LoadImage();
  Asset_M::FilenameHarvester();
  Asset_M::LoadChecker();
  Asset_M::FreeImage();
#endif
  return 1;
}