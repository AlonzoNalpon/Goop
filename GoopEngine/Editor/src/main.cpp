#include <pch.h>

//#define EXCEPTION_TEST
#define ECS_TEST
#ifdef ECS_TEST
#include "../ECS/ECS Example/Scene.h"
#endif // ECS_TEST

//#define GRAPHICS_TEST
#ifdef GRAPHICS_TEST
#include <GLApp/Window/Window.h>
#include <GLApp/Graphics/GraphicsEngine.h>
#endif

#include "../Physics/Physics.h"

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
#ifdef GRAPHICS_TEST
  WindowSystem::Window window{ 800, 800, "GOOP"};
  window.CreateAppWindow();
  Graphics::GraphicsEngine gEngine;
  
  gEngine.Init(Graphics::Colorf{});

  while (!window.GetWindowShouldClose()) {
    gEngine.Draw();
    window.SwapBuffers();
  }
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

  return 1;
}