/*!*********************************************************************
\file   AppController.cpp
\author loh.j@digipen.edu
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

      window = { *am->GetConfigData<int>("Window Width"), *am->GetConfigData<int>("Window Height"), "GOOP" };
      window.CreateAppWindow();

      gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
      am->LoadFiles();
      of.LoadPrefabsFromFile();

      imgui.Init(window);
      
      // of.ObjectFactoryTest();
      im.InitInputManager(window.GetWindow(), *am->GetConfigData<int>("Window Width"), *am->GetConfigData<int>("Window Height"), 0.1);

      GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
      scriptMan->InitMono();

      GE::AI::TestTree();

    }
    catch (GE::Debug::IExceptionBase& e)
    {
      e.LogSource();
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

        fRC.StartSystemTimer();
        im.UpdateInput();
        fRC.EndSystemTimer("Input System");

        fRC.StartSystemTimer();
        imgui.Update();
        fRC.EndSystemTimer("ImGui Update");

        gEngine.ClearBuffer();

        gsm.Update();

        fRC.StartSystemTimer();
        try
        {
          ecs->UpdateSystems();
        }
        catch (GE::Debug::IExceptionBase& e)
        {
          e.LogSource();
        }
        fRC.EndSystemTimer("Scene Update");

        fRC.StartSystemTimer();
        gEngine.Draw();
        fRC.EndSystemTimer("Draw");

        fRC.StartSystemTimer();
        imgui.Render();
        fRC.EndSystemTimer("ImGui Render");

        std::stringstream ss;
        ss << *GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Window Title") << " | FPS: " << std::fixed
          << std::setfill('0') << std::setw(5) << std::setprecision(2) << fRC.GetFPS() << " | Entities: " << EntityComponentSystem::GetInstance().GetEntities().size();
        for (auto const& system : fRC.GetSystemTimers())
        {
          ss << " | " << system.first << ": " << std::setw(4) << system.second.count() << "us";
        }
        window.SetWindowTitle(ss.str().c_str()); // this is how you set window title

        window.SwapBuffers();
        fRC.EndFrame();
      }
      catch (GE::Debug::IExceptionBase& e)
      {
        e.LogSource();
        // This is to ensure if a throw happens and ImGui did not get
        // a chance to render. It will render 1 frame regardless of if
        // has everything it needs to render.
        // This prevents a crash when NewFrame is called without Render() call
        if (!GE::EditorGUI::ImGuiHelper::GetFrameEnded())
        {
          imgui.Render();
        }
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
      e.LogSource();
    }
  }

}