/*!*********************************************************************
\file   ImGuiUI.cpp
\author w.chinkitbryam\@digipen.edu
\date   28 September 2023
\brief
  ImGui Editor UI Wrapper

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>

// run ImGui Demo when in debug mode
#ifdef _DEBUG
#define RUN_IMGUI_DEMO
#endif

#include "ImGuiUI.h"
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include "../ImNode/NodeEditor.h"
#include "../ObjectFactory/ObjectFactory.h"
#include "../Component/Transform.h"
#include "../Component/BoxCollider.h"
#include <Audio/AudioEngine.h>
#include "SceneHierachy.h"
#include "ToolBar.h"
#include "DataViz/Visualizer.h"
#include <Systems/Physics/CollisionSystem.h>
#include "Console.h"
#include "Inspector.h"
#include "SceneControls.h"
#include "AssetBrowser.h"
#include <EditorUI/EditorViewport.h>
#include <EditorUI/PrefabEditor.h>

using namespace GE::EditorGUI;
using namespace DataViz;
using namespace ImGui;

// Initialize static
GE::ECS::Entity ImGuiHelper::m_selectedEntity = GE::ECS::INVALID_ID;
bool ImGuiHelper::m_play = false;
bool ImGuiHelper::m_pause = false;
bool ImGuiHelper::m_step = false;
bool ImGuiHelper::m_restart = false;

void ImGuiUI::Init(WindowSystem::Window& prgmWindow)
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  CreateContext();
  ImGuiIO& io = GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking Controls

  // Setup Dear ImGui style
  StyleColorsDark();

  this->window = &prgmWindow;
  ecs = &GE::ECS::EntityComponentSystem::GetInstance();
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(prgmWindow.GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  GE::AI::NodeEditor* ne = &(GE::AI::NodeEditor::GetInstance());
  ne->NodeEditorInit();
}

void ImGuiUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  NewFrame();

#ifdef RUN_IMGUI_DEMO
  ImGui::ShowDemoWindow();
#endif

  ImGuiHelper::CreateDockSpace("Goop Engine");

  ToolBar::CreateContent();

  SceneControls::CreateContent();

  Begin("Console");
  Console::CreateContent();
  End();

  Begin("Scene Heirachy");
  SceneHierachy::CreateContent();
  End();

  Begin("Inspector");
  Inspector::CreateContent();
  End();

  Begin("Prefab Editor");
  PrefabEditor::CreateContent();
  End();

  Begin("Viewport");
  {
    EditorViewport::UpdateViewport();
  }
  End();

  Begin("Collision Partitioning");
  // for now will be here, can move somehwere else later
  ImGui::InputInt("Change Row", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetRow(), 1);
  ImGui::InputInt("Change Col", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetCol(), 1);
  End();

  Begin("Buttons");
  if (Button("Create MineWorm"))
  {
    GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("MineWorm");
  }
  else if (Button("Clone Latest Object"))
  {
    double randX = static_cast<double>((rand() % window->GetWinWidth()) - window->GetWinWidth() / 2);
    double randY = static_cast<double>((rand() % window->GetWinHeight()) - window->GetWinHeight() / 2);
    GE::ObjectFactory::ObjectFactory::GetInstance().CloneObject(ecs->GetEntities().size() - 1, Math::dVec2(randX, randY));
  }
  else if (Button("Duplicate 500"))
  {
    for (int i{}; i < 500; ++i)
    {
      GE::ECS::Entity entity = ImGuiHelper::GetSelectedEntity();
      try
      {
        double randX = static_cast<double>((rand() % window->GetWinWidth()) - window->GetWinWidth() / 2);
        double randY = static_cast<double>((rand() % window->GetWinHeight()) - window->GetWinHeight() / 2);

        GE::ObjectFactory::ObjectFactory::GetInstance().CloneObject(entity, GE::Math::dVec3{randX, randY, 0});
      }
      catch (GE::Debug::IExceptionBase& ex)
      {
        ex.LogSource();
      }
    }
  }
  End();

  Begin("Asset Browser");
  AssetBrowser::CreateContent();
  End();

  
  if (Visualizer::IsPerformanceShown())
  {
    Visualizer::CreateContent("Performance Graph");
  }

  Begin("Audio");
  Assets::AssetManager const& aM{ Assets::AssetManager::GetInstance() };
  if (Button("Play BGM"))
  {
    Audio::AudioEngine::GetInstance().PlaySound(aM.GetSound("bgm1"), 0.5f, true);
  }
  else if (Button("Play Scream Sound"))
  {
    Audio::AudioEngine::GetInstance().PlaySound(aM.GetSound("JoelScream"), 0.70f);
  }
  else if (Button("DJ Drop Da Beat"))
  {
    Audio::AudioEngine::GetInstance().PlaySound(aM.GetSound("ChengEnBeatbox"), 1.25f, true);
  }
  else if (Button("Play Qurr Sound"))
  {
    Audio::AudioEngine::GetInstance().PlaySound(aM.GetSound("ChengEnQur"), 0.9f);
  }
  else if (Button("Stop Scream Sound"))
  {
    Audio::AudioEngine::GetInstance().StopSound(aM.GetSound("JoelScream"));
  }
  else if (Button("DJ Pick Up Da Beat"))
  {
    Audio::AudioEngine::GetInstance().StopSound(aM.GetSound("ChengEnBeatbox"));
  }
  else if (Button("Stop Qur Sound"))
  {
    Audio::AudioEngine::GetInstance().StopSound(aM.GetSound("ChengEnQur"));
  }
  else if (Button("Stop All Sounds"))
  {
    Audio::AudioEngine::GetInstance().StopAllChannels();
  }
  End();


  GE::AI::NodeEditor* ne = &(GE::AI::NodeEditor::GetInstance());
  ne->NodeEditorShow();

  ImGuiHelper::EndDockSpace();
}

void ImGuiUI::Render()
{
  // Empty function callback
  ErrorCheckEndFrameRecover([](void*, const char*, ...) {GE::Debug::ErrorLogger::GetInstance().LogCritical("ImGui Update failed. Begin stack not ended"); });
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void ImGuiUI::Exit()
{
  GE::AI::NodeEditor* ne = &(GE::AI::NodeEditor::GetInstance());
  ne->NodeEditorShutdown();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();



  DestroyContext();
}

void ImGuiHelper::CreateDockSpace(const char* projectName)
{
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  const ImGuiViewport* viewport = GetMainViewport();
  SetNextWindowPos(viewport->WorkPos);
  SetNextWindowSize(viewport->WorkSize);
  SetNextWindowViewport(viewport->ID);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
  // and handle the pass-thru hole, so we ask Begin() to not render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  Begin(projectName, nullptr, window_flags);

  // Submit the DockSpace
  ImGuiIO& io = GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dockspace_id = GetID("MyDockSpace");
    DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }
}

void GE::EditorGUI::ImGuiHelper::EndDockSpace()
{
  End();
}

GE::ECS::Entity GE::EditorGUI::ImGuiHelper::GetSelectedEntity()
{
  return m_selectedEntity;
}

void GE::EditorGUI::ImGuiHelper::SetSelectedEntity(GE::ECS::Entity& selectedEntity)
{
  m_selectedEntity = selectedEntity;
}

void GE::EditorGUI::ImGuiHelper::StepSimulation(bool shouldStep)
{
  m_step = shouldStep;
}

bool GE::EditorGUI::ImGuiHelper::StepSimulation()
{
  bool shouldStep = m_step;
  m_step = false;
  return shouldStep;
}

bool GE::EditorGUI::ImGuiHelper::ShouldPlay()
{
  return m_play;
}

void GE::EditorGUI::ImGuiHelper::Play()
{
  m_play = true;
  m_step = false;
  m_pause = true;
}

void GE::EditorGUI::ImGuiHelper::Pause()
{
  m_pause = true;
  m_play = false;
}

bool GE::EditorGUI::ImGuiHelper::Paused()
{
  return m_pause;
}

void GE::EditorGUI::ImGuiHelper::Restart()
{
  m_restart = true;
  m_play = false;
  m_step = false;
}

bool GE::EditorGUI::ImGuiHelper::IsRunning()
{
  return m_play;
}

bool GE::EditorGUI::ImGuiHelper::ShouldRestart()
{
  bool shouldRestart = m_restart;
  m_restart = false;
  return shouldRestart;
}
