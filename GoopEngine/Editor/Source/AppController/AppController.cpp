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
#include <EditorUI/DataViz/Visualizer.h>

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
      
      of.ObjectFactoryTest();
      im.InitInputManager(window.GetWindow(), *am->GetConfigData<int>("Window Width"), *am->GetConfigData<int>("Window Height"), 0.1);

      GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
      scriptMan->InitMono();

      GE::Debug::ErrorLogger::GetInstance().SuppressLogMessages(true);

      GE::AI::TestTree();

    }
    catch (GE::Debug::IExceptionBase& e)
    {
      e.LogSource();
    }
  }
  
  void AppController::Run()
  {
    try
    {
      GE::ECS::EntityComponentSystem *ecs = { &GE::ECS::EntityComponentSystem::GetInstance() };
      //scn.Start();
      gsm.Init();

      while (!window.GetWindowShouldClose())
      {
        fRC.StartFrame();

        fRC.StartSystemTimer();
        im.UpdateInput();
        fRC.EndSystemTimer("Input System");

        static bool renderUI = false;
        if (Input::InputManager::GetInstance().IsKeyTriggered(GPK_G))
        {
          renderUI = !renderUI;
        }

        fRC.StartSystemTimer();
        if (renderUI)
        {
          imgui.Update();
        }
        fRC.EndSystemTimer("ImGui Update");
        gEngine.ClearBuffer();

        fRC.StartSystemTimer();
        ecs->UpdateSystems();
        //scn.Update();
        fRC.EndSystemTimer("Scene Update");

        fRC.StartSystemTimer();
        gEngine.Draw();
        fRC.EndSystemTimer("Draw");

        fRC.StartSystemTimer();
        if (renderUI)
        {
          imgui.Render();
        }
        fRC.EndSystemTimer("ImGui Render");
        
        // update graph for system timers if window is shown
        if (EditorGUI::DataViz::Visualizer::IsPerformanceShown())
        {
          EditorGUI::DataViz::Visualizer::UpdateSystemTimes();
        }

        std::stringstream ss;
        ss << *GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Window Title") << " | FPS: " << std::fixed
          << std::setfill('0') << std::setw(5) << std::setprecision(2) << fRC.GetFPS() << " | Entities: " << EntityComponentSystem::GetInstance().GetEntities().size();
        for (auto system : fRC.GetSystemTimers())
        {
          ss << " | " << system.first << ": " << std::setw(4) << system.second.count() << "us";
        }
        window.SetWindowTitle(ss.str().c_str()); // this is how you set window title

        window.SwapBuffers();
        fRC.EndFrame();
      }
    }
    catch (GE::Debug::IExceptionBase& e)
    {
      e.LogSource();
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