/*!*********************************************************************
\file   AppController.cpp
\author loh.j\@digipen.edu
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
#include <Prefabs/PrefabManager.h>
#ifndef IMGUI_DISABLE
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <EditorUI/EditorViewport.h>
#endif

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

  /*!*********************************************************************
  \brief
    Wrapper function to print out exceptions.

  \param e
    Exception caught
  ************************************************************************/
  void PrintException(std::exception& e);
}

namespace GE::Application
{
  AppController::AppController() :
    window{ 0, 0, "Goop Engine" },
    gEngine{ Graphics::GraphicsEngine::GetInstance() },
    fRC{ GE::FPS::FrameRateController::GetInstance() },
    im{ GE::Input::InputManager::GetInstance() },
    gsm{ GE::GSM::GameStateManager::GetInstance() },
    fMod{ GE::fMOD::FmodSystem::GetInstance() }
  {}
  
  void AppController::Init()
  {
    GE::Events::EventManager::GetInstance().Subscribe<GE::Events::QuitGame>(this);
    GE::Events::EventManager::GetInstance().Subscribe<GE::Events::ToggleFullscreen>(this);
    GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowMinimize>(this);

    // INIT FUNCTIONS
    GE::Assets::AssetManager* am = &GE::Assets::AssetManager::GetInstance();
    am->LoadConfigData("./Assets/Config.cfg");
    GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };
    of.RegisterComponentsAndSystems();

    GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
    scriptMan->InitMono();

    GE::AI::TreeManager::GetInstance().Init();

    GE::Events::EventManager::GetInstance().SubscribeAllListeners();

    fRC.InitFrameRateController(am->GetConfigData<int>("FPS Limit"), am->GetConfigData<int>("Steps Per Second"), am->GetConfigData<int>("FPS Check Interval"));

    // window = { am->GetConfigData<int>("Window Width"), am->GetConfigData<int>("Window Height"), "Goop Engine"};
    // Now we create window with detected resolution
    
    window.CreateAppWindow();

    gEngine.Init(Graphics::Colorf{ }, window.GetWinWidth(), window.GetWinHeight()); // Initialize the engine with this clear color
    am->LoadFiles();

    Prefabs::PrefabManager::GetInstance().LoadPrefabsFromFile();

#ifndef IMGUI_DISABLE
    imgui.Init(window);
#endif
    // Creation of game framebuffer MUST OCCUR AFTER IMGUI INIT (editor has their own)
    gEngine.CreateFrameBuffer(window.GetWinWidth(), window.GetWinHeight());
    //gEngine.CreateFrameBuffer(1920, 1080);

    fMod.Init();

    im.InitInputManager(window.GetWindow(), window.GetWinWidth(), window.GetWinHeight(), 0.1);
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
          fMod.Update();
          fRC.EndSystemTimer("Fmod");
          im.UpdateInput();
          fRC.EndSystemTimer("Input");

#ifndef IMGUI_DISABLE
          // Enable/disable editor view with G key
          if (im.IsKeyTriggered(KEY_CODE::KEY_G) && EditorGUI::EditorViewport::isFocused)
          {
            showEditor = !showEditor;
            im.SetCurrFramebuffer(showEditor == true? 0:1);
          }

          fRC.StartSystemTimer();
          if (showEditor)
            imgui.Update();

          fRC.EndSystemTimer("ImGui Update");

          if (GE::EditorGUI::ImGuiHelper::IsRunning())
          {
            ecs->UpdateSystems();
          }
          else if (GE::EditorGUI::ImGuiHelper::StepSimulation())
          {
            ecs->UpdateSystems();
          }
          else
          {
            ecs->UpdateSystems(4,
              typeid(GE::Systems::CollisionSystem).name(),
              typeid(GE::Systems::RenderSystem).name(),
              typeid(GE::Systems::PreRootTransformSystem).name(),
              typeid(GE::Systems::PostRootTransformSystem).name());
          }
#else
          ecs->UpdateSystems();
#endif  // IMGUI_DISABLE
        }
        catch (GE::Debug::IExceptionBase& e)
        {
          PrintException(e);
        }
        catch (std::exception& e)
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
          fRC.EndSystemTimer("Deferred Render");

#ifndef IMGUI_DISABLE
          fRC.StartSystemTimer();
          if (showEditor)
            imgui.Render();
          else
            Graphics::GraphicsEngine::GetInstance().RenderToScreen(1);
          fRC.EndSystemTimer("ImGui Render");
#else
          // RENDERING TO SCREEN WITHOUT IMGUI
          gEngine.RenderToScreen();
#endif
        }
        catch (GE::Debug::IExceptionBase& e)
        {
          PrintException(e);
        }
        catch (std::exception& e)
        {
          PrintException(e);
        }

        window.SwapBuffers();
        gsm.Update();
        fRC.EndFrame();
      }
      catch (GE::Debug::IExceptionBase& e)
      {
        PrintException(e);
      }
      catch (std::exception& e)
      {
        PrintException(e);
      }
    }
  }
  
  void AppController::Exit()
  {

    try
    {
      GE::AI::TreeManager::GetInstance().ShutDown();
      gsm.Exit();
#ifndef IMGUI_DISABLE
      imgui.Exit();
#endif
    }
    catch (GE::Debug::IExceptionBase& e)
    {
      PrintException(e);
    }
  }

  void AppController::HandleEvent(GE::Events::Event* e)
  {
    if (dynamic_cast<GE::Events::QuitGame*>(e))
      glfwSetWindowShouldClose(window.GetWindow(), 1);
    else if (dynamic_cast<GE::Events::ToggleFullscreen*>(e))
      window.ToggleFullscreen();
    else if (dynamic_cast<GE::Events::WindowMinimize*>(e))
      window.MinimizeWindow();
  }

}

namespace
{
  void PrintException(GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
  }

  void PrintException(std::exception& e)
  {
    GE::Debug::ErrorLogger::GetInstance().LogCritical(e.what());
  }
}