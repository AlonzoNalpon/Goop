/*!*********************************************************************
\file   AppController.cpp
\author loh.j@digipen.edu
\co-author w.chinkitbryan\@digipen.edu
\date   18-September-2023
\brief
  The AppController class is the main controller for running the
  application.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <AppController/AppController.h>
#include "../EditorUI/ImGuiUI.h"

using namespace GE::ECS;

namespace
{
  /*!*********************************************************************
  \brief 
    Wrapper function to print out exceptions.

  \param e
    Exception caught
  ************************************************************************/
  void PrintException(GE::Debug::IExceptionBase& e);
}

namespace GE::Application
{
  AppController::AppController() :
    window{ 0, 0, nullptr },
    gEngine{ Graphics::GraphicsEngine::GetInstance() },
    fRC{ GE::FPS::FrameRateController::GetInstance() },
    im{ GE::Input::InputManager::GetInstance() },
    gsm{}
  {}
  
  void AppController::Init()
  {
    // INIT FUNCTIONS
    try
    {
      GE::Assets::AssetManager* am = &GE::Assets::AssetManager::GetInstance();
      am->LoadConfigData("./Assets/Config.cfg");
      GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };
      of.RegisterComponentsAndSystems();

      GE::Events::EventManager::GetInstance().SubscribeAllListeners();

      fRC.InitFrameRateController(*am->GetConfigData<int>("FPS Limit"), *am->GetConfigData<int>("Steps Per Second"), *am->GetConfigData<int>("FPS Check Interval"));

      window = { *am->GetConfigData<int>("Window Width"), *am->GetConfigData<int>("Window Height"), "Goop Engine"};
      window.CreateAppWindow();

      gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
      am->LoadFiles();
      of.LoadPrefabsFromFile();

      imgui.Init(window);
      
      // of.ObjectFactoryTest();
      im.InitInputManager(window.GetWindow(), *am->GetConfigData<int>("Window Width"), *am->GetConfigData<int>("Window Height"), 0.1);

      GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
      scriptMan->InitMono();


    }
    catch (GE::Debug::IExceptionBase& e)
    {
      PrintException(e);
    }
  }
  
  void AppController::Run()
  {
    GE::ECS::EntityComponentSystem *ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
    gsm.Init();

    while (!window.GetWindowShouldClose())
    {
      // Try catch in the loop so the program can still run
      try
      {
        fRC.StartFrame();

        gEngine.ClearBuffer();

        /////////////////
        // Update calls
        /////////////////
        try
        {
          fRC.StartSystemTimer();
          im.UpdateInput();
          fRC.EndSystemTimer("Input");

          fRC.StartSystemTimer();
          imgui.Update();
          fRC.EndSystemTimer("ImGui Update");

          gsm.Update();

          fRC.StartSystemTimer();
          ecs->UpdateSystems();
          fRC.EndSystemTimer("Scene Update");
        }
        catch (GE::Debug::IExceptionBase& e)
        {
          PrintException(e);
        }

        /////////////////
        // Draw calls
        /////////////////
        try
        {
          fRC.StartSystemTimer();
          gEngine.Draw();
          fRC.EndSystemTimer("Draw");

          fRC.StartSystemTimer();
          imgui.Render();
          fRC.EndSystemTimer("ImGui Render");
        }
        catch (GE::Debug::IExceptionBase& e)
        {
          PrintException(e);
        }

        window.SwapBuffers();
        fRC.EndFrame();
      }
      catch (GE::Debug::IExceptionBase& e)
      {
        PrintException(e);
      }
    }
  }
  
  void AppController::Exit()
  {
    try
    {
      gsm.Exit();
      imgui.Exit();
    }
    catch (GE::Debug::IExceptionBase& e)
    {
      PrintException(e);
    }
  }

}

namespace
{
  void PrintException(GE::Debug::IExceptionBase& e)
  {
    std::string defError{e.what()};
    if (defError.length() > 0)
    {
      GE::Debug::ErrorLogger::GetInstance().LogCritical(defError);
    }
    else
    {
      e.LogSource();
    }
  }
}