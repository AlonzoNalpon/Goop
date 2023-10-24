/*!*********************************************************************
\file   ImGuiUI.cpp
\author w.chinkitbryam\@digipen.edu
\date   28 September 2023
\brief
  ImGui Editor UI Wrapper

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>

//#define RUN_IMGUI_DEMO  // Uncomment to replace imgui window with demo

#include "ImGuiUI.h"
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>
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
#include "AssetBrowser.h"

using namespace GE::EditorGUI;
using namespace DataViz;
using namespace ImGui;

#define RUN_IMGUI_DEMO

// Initialize static
GE::ECS::Entity ImGuiHelper::m_selectedEntity = GE::ECS::INVALID_ID;

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

  Begin("Scene Heirachy");
  SceneHierachy::CreateContent();
  End();

  Begin("Viewport");
  auto& gEngine = Graphics::GraphicsEngine::GetInstance();
  GLuint texture = gEngine.GetRenderTexture();
  ImVec2 viewportSize = ImGui::GetContentRegionAvail();
  // Define the UV coordinates for the flipped rendering
  ImVec2 uv0 = ImVec2(1.0f, 0.0f); // Top-right
  ImVec2 uv1 = ImVec2(0.0f, 1.0f); // Bottom-left
  ImGui::Image((void*)(intptr_t)texture, viewportSize, uv1, uv0);
  End();

  Begin("Collision Partitioning");
  // for now will be here, can move somehwere else later
  ImGui::InputInt("Change Row", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetRow(), 1);
  ImGui::InputInt("Change Col", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetCol(), 1);
  End();

  Begin("Asset Browser");
  if (Button("Create MineWorm"))
  {
    GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("MineWorm");
  }
  else if (Button("Clone Object"))
  {
    double randX = static_cast<double>((rand() % window->GetWinWidth()) - window->GetWinWidth() / 2);
    double randY = static_cast<double>((rand() % window->GetWinHeight()) - window->GetWinHeight() / 2);
    GE::ObjectFactory::ObjectFactory::GetInstance().CloneObject(6, Math::dVec2(randX, randY));
  }
  else if (Button("Create 2.5k Render"))
  {
    for (int i{}; i < 2500; ++i)
    {
      try
      {
        GE::ECS::Entity entity = GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("Buta PIG");
        GE::Component::Transform* trans = ecs->GetComponent<GE::Component::Transform>(entity);
        GE::Component::BoxCollider* box = ecs->GetComponent<GE::Component::BoxCollider>(entity);
        if (trans)
        {
          double randX = static_cast<double>((rand() % window->GetWinWidth()) - window->GetWinWidth() / 2);
          double randY = static_cast<double>((rand() % window->GetWinHeight()) - window->GetWinHeight() / 2);
          trans->m_pos = Math::dVec2(randX, randY);
          box->m_center = trans->m_pos;
        }
      }
      catch (GE::Debug::IExceptionBase& ex)
      {
        ex.LogSource();
      }
    }
  }
  End();

  Begin("Asset Browser(Tree)");
  AssetBrowser::CreateContent();
  End();

  
  if (Visualizer::IsPerformanceShown())
  {
    Visualizer::UpdatePerformanceTab("Performance Visualizer");
  }

#ifdef _DEBUG
  static bool showOverlay = true;
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowWidth() / 2.f, 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(300, 30), ImGuiCond_Always);

  ImGui::Begin("Overlay Window", &showOverlay, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  ImGui::Text("Mouse Pos: (%.2f, %.2f)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

  ImGui::End();
#endif

  Begin("Audio");
  if (Button("Play Scream Sound"))
  {
    Audio::AudioEngine::GetInstance().PlaySound("Assets/JoelScream.wav", 0.70f);
  }
  else if (Button("DJ Drop Da Beat"))
  {
    Audio::AudioEngine::GetInstance().PlaySound("Assets/ChengEnBeatbox.wav", 1.25f, true);
  }
  else if (Button("Play Qurr Sound"))
  {
    Audio::AudioEngine::GetInstance().PlaySound("Assets/ChengEnQur.wav", 0.9f);
  }
  else if (Button("Stop Scream Sound"))
  {
    Audio::AudioEngine::GetInstance().StopSound("Assets/JoelScream.wav");
  }
  else if (Button("DJ Pick Up Da Beat"))
  {
    Audio::AudioEngine::GetInstance().StopSound("Assets/ChengEnBeatbox.wav");
  }
  else if (Button("Stop Qur Sound"))
  {
    Audio::AudioEngine::GetInstance().StopSound("Assets/ChengEnQur.wav");
  }
  else if (Button("Stop All Sounds"))
  {
    Audio::AudioEngine::GetInstance().StopAllChannels();
  }
  Audio::AudioEngine::GetInstance().Update();
  End();

  Begin("Inspector");
  Inspector::CreateContent();
  End();

  Begin("Console");
  Console::CreateContent();
  End();

  ImGuiHelper::EndDockSpace();
}

void ImGuiUI::Render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void ImGuiUI::Exit()
{
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
