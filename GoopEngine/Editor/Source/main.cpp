#include <pch.h>
#include <AppController/AppController.h>

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetBreakAlloc(208521);
#endif

  GE::Application::AppController ac;

  try
  {
    ac.Init();
    ac.Run();
    ac.Exit();
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
  }
  catch (std::exception& e)
  {
    GE::Debug::ErrorLogger::GetInstance().LogCritical(e.what());
  }

  return 0;
}