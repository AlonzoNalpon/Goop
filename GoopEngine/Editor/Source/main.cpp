/*!*********************************************************************
\file   main.cpp
\date   10-October-2023
\brief  The entry point of the whole program. Creates an instance of
        the AppController and calls its functions to run initialize
        and run all systems and their behaviours. If any execption is
        thrown, it will be logged accordingly by the error logger.

        In debug mode, a macro is used to log memory leaks.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <AppController/AppController.h>

int main(int /*argc*/, char* /*argv*/[])
{
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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