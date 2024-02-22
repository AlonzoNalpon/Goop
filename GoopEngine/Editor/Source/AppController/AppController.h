/*!*********************************************************************
\file   AppController.h
\author loh.j\@digipen.edu
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
#include <AI/TreeManager.h>
#include <GameStateManager/GameStateManager.h>
#include <Fmod/FmodSystem.h>
#include <Events/Listener.h>

namespace GE::Application
{
  // AppController singleton
  class AppController : public GE::Events::IEventListener
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
    Graphics::GraphicsEngine& gEngine;
    GE::FPS::FrameRateController& fRC;
#ifndef IMGUI_DISABLE
    GE::EditorGUI::ImGuiUI imgui;
    bool showEditor{true}; // !< flag for whether editor should be rendered (vs game camera)
#endif
    GE::Input::InputManager& im;
    GE::GSM::GameStateManager& gsm;
    GE::fMOD::FmodSystem& fMod;

    void HandleEvent(GE::Events::Event* e) override;
  };
}
