/*!*********************************************************************
\file   AppController.h
\author loh.j@digipen.edu
\date   18-September-2023
\brief
  The AppController class is the main controller for running the 
  application. 

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Window/Window.h>
#include <AssetManager/AssetManager.h>
#include <ObjectFactory/ObjectFactory.h>
#include <Graphics/GraphicsEngine.h>
#include <MemoryManager/MemoryManager.h>
#include <EditorUI/ImGuiUI.h>
#include <ScriptEngine/ScriptManager.h>
#include <Scene.h>


namespace GE::Application
{
  // AppController singleton
  class AppController
  {
  public:
    /*!*********************************************************************
    \brief
      Constructor of the AppController class.
    ************************************************************************/
    AppController();

    /*!*********************************************************************
    \brief
      Initializes all the required singletons and classes to start the 
      engine.
    ************************************************************************/
    void Init();

    /*!*********************************************************************
    \brief
      The main loop of the engine.
    ************************************************************************/
    void Run();

    /*!*********************************************************************
    \brief
      Exit function to stop the engine.
    ************************************************************************/
    void Exit();

  private:
    WindowSystem::Window window;
    GE::EditorGUI::ImGuiUI imgui;
    Scene scn;
    GE::FPS::FrameRateController& fRC;
    GE::Input::InputManager* im;
    Graphics::GraphicsEngine& gEngine;
  };
}
