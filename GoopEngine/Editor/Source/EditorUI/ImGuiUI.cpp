/*!*********************************************************************
\file   ImGuiUI.cpp
\author w.chinkitbryan\@digipen.edu
\date   28-September-2023
\brief
  ImGui Editor UI Wrapper

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
// run ImGui Demo when in debug mode
#ifdef _DEBUG
#define RUN_IMGUI_DEMO
#endif

#include "ImGuiUI.h"
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include "../ImNode/NodeEditor.h"
#include <ObjectFactory/ObjectFactory.h>
#include <Component/Transform.h>
#include <Component/BoxCollider.h>
#include <Systems/Audio/AudioSystem.h>
#include "SceneHierachy.h"
#include "ToolBar.h"
#include "DataViz/Visualizer.h"
#include <Systems/Physics/CollisionSystem.h>
#include "Console.h"
#include "Inspector.h"
#include "SceneControls.h"
#include "AssetBrowser.h"
#include <EditorUI/EditorViewport.h>
#include <EditorUI/AssetPreview.h>
#include <GameStateManager/GameStateManager.h>
#include <Commands/CommandManager.h>
#include <ImGuizmo_1_83/ImGuizmo.h>
#include <EditorUI/GizmoEditor.h>
#include <EditorUI/SpriteAnimEditor.h>
#include <EditorUI/AnimEventEditor.h>
#include <EditorUI/EditorTheme.h>
#include <EditorUI/SpriteSheetEditor.h>

using namespace GE::EditorGUI;
using namespace DataViz;
using namespace ImGui;

// Initialize static
GE::ECS::Entity ImGuiHelper::m_selectedEntity = GE::ECS::INVALID_ID;
std::filesystem::path ImGuiHelper::m_selectedAsset;
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

  // Set up editor framebuffer for viewport
  auto& gEngine{ Graphics::GraphicsEngine::GetInstance() };
  frameBuffer.first = gEngine.CreateFrameBuffer(window->GetWinWidth(), window->GetWinHeight());
  frameBuffer.second = &gEngine.GetFrameBuffer(frameBuffer.first);

  ecs = &GE::ECS::EntityComponentSystem::GetInstance();
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(prgmWindow.GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  GE::AI::NodeEditor* ne = &(GE::AI::NodeEditor::GetInstance());
  ne->NodeEditorInit();
  GE::EditorGUI::EditorTheme::SetImGuiTheme();
}

void ImGuiUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  NewFrame();

  GE::AI::NodeEditor* ne = &(GE::AI::NodeEditor::GetInstance());
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

  Begin("Node editor");
  ne->NodeEditorShow();
  End();

  Begin("Asset Preview");
  AssetPreview::CreateContent();
  End();

  Begin("Viewport");
  {
    ImGuizmo::SetOrthographic(true);
    ImGuizmo::BeginFrame(); // call the imguizmo new frame function
    ImGuizmo::SetDrawlist(); // internally set draw list for imguizmo
    auto& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    ImGuizmo::Enable(true);

    EditorViewport::RenderViewport(*frameBuffer.second);
    if (GizmoEditor::isVisible())
    {
      frameBuffer.second->camera.CalculateProjMtx();
      frameBuffer.second->camera.CalculateViewProjMtx();
      GizmoEditor::UpdateGizmoMtx(*frameBuffer.second);
      GizmoEditor::RenderGizmo(); // MUST RENDER AFTER VIEWPORT
      GizmoEditor::PostRenderUpdate();
    }
    EditorViewport::UpdateViewport(*frameBuffer.second);
    GizmoEditor::SetVisible(false); // reset state
  }
  End();

  //Begin("Animation Editor");
  //End();
  Begin("Sprite Animation Events Editor");
  EditorGUI::AnimEventEditor::CreateContent();
  End();
  
  if (SpriteSheetEditor::IsToggled())
  {
    EditorGUI::SpriteSheetEditor::CreateContent("Sprite Sheet Editor");
  }

  Begin("Extras");
  ImGui::InputInt("Change Row", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetRow(), 1);
  ImGui::InputInt("Change Col", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetCol(), 1);


  static GE::fMOD::FmodSystem& fMod = GE::fMOD::FmodSystem::GetInstance();
  if (Button("Stop all sounds"))
  {
    fMod.StopAllSound();
  }
  if (Button("Stop BGM"))
  {
    fMod.StopChannel(GE::fMOD::FmodSystem::BGM);
  }
  if (Button("Stop SFX"))
  {
    fMod.StopChannel(GE::fMOD::FmodSystem::SFX);
  }
  if (Button("Stop Voices"))
  {
    fMod.StopChannel(GE::fMOD::FmodSystem::VOICE);
  }
  if (Button("Play sound"))
  {
    fMod.PlaySound("menu", 1.0, GE::fMOD::FmodSystem::BGM);
  }

  for (int i{}; i < GE::fMOD::FmodSystem::TOTAL_CHANNELS; ++i)
  {
    GE::fMOD::FmodSystem::ChannelType channeltype = static_cast<GE::fMOD::FmodSystem::ChannelType>(i);
    float vol = fMod.GetChannelVolume(channeltype);
    if (SliderFloat(fMod.m_channelToString.at(channeltype).c_str(), &vol, 0.f, 1.f))
    {
      fMod.SetChannelVolume(channeltype, vol);
    }
  }
  float mvol = fMod.GetMasterVolume();
  if (SliderFloat("Master Volume", &mvol, 0.f, 1.f))
  {
    fMod.SetMasterVolume(mvol);
  }

  End();

  Begin("Asset Browser");
  AssetBrowser::CreateContent();
  End();

  
  if (Visualizer::IsPerformanceShown())
  {
    Visualizer::CreateContent("Performance Graph");
  }

  /*Begin("Audio");
  Assets::AssetManager const& aM{ Assets::AssetManager::GetInstance() };
  if (Button("Play BGM"))
  {
    Audio::AudioSystem::GetInstance().PlaySound(aM.GetSound("bgm1"), 0.5f, true);
  }
  else if (Button("Play Scream Sound"))
  {
    Audio::AudioSystem::GetInstance().PlaySound(aM.GetSound("JoelScream"), 0.70f);
  }
  else if (Button("DJ Drop Da Beat"))
  {
    Audio::AudioSystem::GetInstance().PlaySound(aM.GetSound("ChengEnBeatbox"), 1.25f, true);
  }
  else if (Button("Play Qurr Sound"))
  {
    Audio::AudioSystem::GetInstance().PlaySound(aM.GetSound("ChengEnQur"), 0.9f);
  }
  else if (Button("Stop Scream Sound"))
  {
    Audio::AudioSystem::GetInstance().StopSound(aM.GetSound("JoelScream"));
  }
  else if (Button("DJ Pick Up Da Beat"))
  {
    Audio::AudioSystem::GetInstance().StopSound(aM.GetSound("ChengEnBeatbox"));
  }
  else if (Button("Stop Qur Sound"))
  {
    Audio::AudioSystem::GetInstance().StopSound(aM.GetSound("ChengEnQur"));
  }
  else if (Button("Stop All Sounds"))
  {
    Audio::AudioSystem::GetInstance().StopAllChannels();
  }
  End();*/

  if (ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift && IsKeyPressed((ImGuiKey)GLFW_KEY_Z)) {
    GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
    cmdMan.Redo();
    SetWindowFocus();
  }

  else if (ImGui::GetIO().KeyCtrl && IsKeyPressed((ImGuiKey)GLFW_KEY_Z)) {
    GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
    cmdMan.Undo();
    SetWindowFocus();
  }




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

std::filesystem::path const& GE::EditorGUI::ImGuiHelper::GetSelectedAsset()
{
  return m_selectedAsset;
}

void GE::EditorGUI::ImGuiHelper::SetSelectedAsset(std::filesystem::path const& selectedAssetPath)
{
  m_selectedAsset = selectedAssetPath;
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
  m_pause = false;
}

bool GE::EditorGUI::ImGuiHelper::IsRunning()
{
  return m_play;
}

bool GE::EditorGUI::ImGuiHelper::IsStopped()
{
  return !m_play && !m_pause;
}

bool GE::EditorGUI::ImGuiHelper::ShouldRestart()
{
  bool shouldRestart = m_restart;
  m_restart = false;
  return shouldRestart;
}
#endif