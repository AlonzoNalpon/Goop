#include <pch.h>
#include <AppController/AppController.h>

int main(int /*argc*/, char* /*argv*/[])
{
  srand(static_cast<unsigned int>(time(NULL)));
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  GE::Application::AppController ac;

  ac.Init();

  ac.Run();

  ac.Exit();

  return 1;
}