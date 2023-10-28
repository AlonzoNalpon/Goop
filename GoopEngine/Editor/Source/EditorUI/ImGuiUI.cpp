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
#include "SceneControls.h"
#include "AssetBrowser.h"

using namespace GE::EditorGUI;
using namespace DataViz;
using namespace ImGui;

#define RUN_IMGUI_DEMO

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

  Begin("Viewport");
  {
    ImGuiHelper::UpdateViewport();
  }
  End();

  Begin("Collision Partitioning");
  // for now will be here, can move somehwere else later
  ImGui::InputInt("Change Row", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetRow(), 1);
  ImGui::InputInt("Change Col", &ecs->GetSystem<GE::Systems::CollisionSystem>()->GetCol(), 1);
  End();

  Begin("Assets");
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
  else if (Button("Create 2.5k Render"))
  {
    for (int i{}; i < 2500; ++i)
    {
      try
      {
        GE::ECS::Entity entity = GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("ButaPIG");
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

  Begin("Asset Browser");
  AssetBrowser::CreateContent();
  End();

  
  if (Visualizer::IsPerformanceShown())
  {
    Visualizer::CreateContent("Performance Visualizer");
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
  Audio::AudioEngine::GetInstance().Update();
  End();

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

void GE::EditorGUI::ImGuiHelper::UpdateViewport()
{
  auto* ecs = &GE::ECS::EntityComponentSystem::GetInstance();
  auto& gEngine = Graphics::GraphicsEngine::GetInstance();
  GLuint texture = gEngine.GetRenderTexture();

  // Calculate the UV coordinates based on viewport position and size
  // Get the size of the GLFW window
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 windowSize{ io.DisplaySize.x, io.DisplaySize.y };
  ImVec2 viewportSize = ImGui::GetContentRegionAvail();  // Get the top-left position of the viewport
  ImVec2 viewportPosition = ImGui::GetCursorScreenPos();
  ImVec2 viewportEnd = ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y);
  ImVec2 uv0;
  ImVec2 uv1;

  uv0.x = 1.f + (viewportEnd.x - windowSize.x) / windowSize.x;
  uv1.y = -(viewportPosition.y) / windowSize.y;
  uv0.y = -(1.f + (viewportEnd.y - windowSize.y) / windowSize.y);
  uv1.x = (viewportPosition.x) / windowSize.x;
  // render the image
  ImGui::Image((void*)(intptr_t)texture, viewportSize, uv1, uv0);

  auto& renderer = gEngine.GetRenderer(); // renderer for setting camera
  if (ImGui::IsMouseHoveringRect(viewportPosition, ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y)))
  {
    ImVec2 mousePosition = ImGui::GetMousePos();
    mousePosition.y = windowSize.y - mousePosition.y;
    static Graphics::gVec2 prevPos; // previous mouse position

    Graphics::gVec2 MousePosF{ mousePosition.x, mousePosition.y }; // current position of mouse in float
    // If the middle mouse button is down (for moving camera)
    {
      constexpr int MIDDLE_MOUSE{ 2 };        // constant for middle mouse key
      static bool middleMouseHeld{};          // flag for middle mouse held down
      if (ImGui::IsMouseDown(MIDDLE_MOUSE))
      {

        if (middleMouseHeld) // check if it's not the first frame button is held
        {
          Graphics::gVec2 displacement{ prevPos - MousePosF };  // displacement of camera
          Graphics::Rendering::Camera& camera{ renderer.GetCamera() }; // get reference to camera
          camera.DisplaceCam({ displacement.x, displacement.y, 0.f });
          prevPos = { MousePosF };
        }
        else // else this is the first frame we are holding it down
        {
          prevPos = { MousePosF };
          middleMouseHeld = true;
        }
      }
      else {
        middleMouseHeld = false;
      }
    }
    // If the mousewheel is scrolled (for zooming camera)
    {
      float scrollValue = io.MouseWheel;
      constexpr float MULTIPLIER = 10.f;
      if (scrollValue)
      {
        Graphics::Rendering::Camera& camera{ renderer.GetCamera() }; // get reference to camera
        camera.ZoomCamera(scrollValue * MULTIPLIER);
      }
    }

    // If the mouse clicked was detected
    {
      constexpr int MOUSE_L_CLICK{ 0 };        // constant for mouse left click
      static bool mouseLHeld{};
      if (ImGui::IsMouseDown(MOUSE_L_CLICK))
      {
        if (!mouseLHeld)
        {
          mouseLHeld = true;
          auto mouseWS{ gEngine.ScreenToWS(MousePosF) };
          double depthVal{ std::numeric_limits<double>::lowest() };
          GE::ECS::Entity selectedID = GE::ECS::INVALID_ID;

          for (GE::ECS::Entity curr : ecs->GetEntities())
          {
            // get sprite component
            auto const* transPtr = ecs->GetComponent<GE::Component::Transform>(curr);
            auto const& trans{ *transPtr };
            GE::Math::dVec2 min{ trans.m_pos.x - trans.m_scale.x * 0.5, trans.m_pos.y - trans.m_scale.y * 0.5 };
            GE::Math::dVec2 max{ trans.m_pos.x + trans.m_scale.x * 0.5, trans.m_pos.y + trans.m_scale.y * 0.5 };

            // AABB check with the mesh based on its transform (ASSUMES A SQUARE)
            if (min.x > mouseWS.x ||
              max.x < mouseWS.x ||
              min.y > mouseWS.y ||
              max.y < mouseWS.y)
              continue;

            // Depth check (only take frontmost object
            if (trans.m_pos.z > depthVal)
            {
              depthVal = trans.m_pos.z;
              selectedID = curr;
            }
          }
          // Set selected entity (invalid ID means none selected)
          ImGuiHelper::SetSelectedEntity(selectedID);
        }
      }
      else
        mouseLHeld = false;
    }
  }
}
