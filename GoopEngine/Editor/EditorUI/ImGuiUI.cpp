#include "ImGuiUI.h"
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include "../ObjectFactory/ObjectFactory.h"

using namespace GE::EditorGUI;
using namespace ImGui;

void ImGuiUI::Init(WindowSystem::Window& window)
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  CreateContext();
  ImGuiIO& io = GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking Controls

  // Setup Dear ImGui style
  StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
  ImGui_ImplOpenGL3_Init();
}

void ImGuiUI::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  NewFrame();

  ImGuiHelper::CreateDockSpace();

  Begin("Scene Heirachy");
  Separator();
  End();

  Begin("Viewport");
  End();

  Begin("Asset Browser");
  if (Button("Create Object1"))
  {
    // TODO
    GE::ObjectFactory::ObjectFactory::GetInstance().SpawnPrefab("MineWorm");
  }
  End();

  Begin("Inspector");
  End();

  End();
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

void ImGuiHelper::CreateDockSpace()
{
  // Remove fullscreen toggle once we have a FBO
  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen)
  {
    const ImGuiViewport* viewport = GetMainViewport();
    SetNextWindowPos(viewport->WorkPos);
    SetNextWindowSize(viewport->WorkSize);
    SetNextWindowViewport(viewport->ID);
    PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }
  else
  {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
  // and handle the pass-thru hole, so we ask Begin() to not render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
  // all active windows docked into it will lose their parent and become undocked.
  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
  if (!opt_padding)
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  Begin("Goop Engine", nullptr, window_flags);
  if (!opt_padding)
    PopStyleVar();

  if (opt_fullscreen)
    PopStyleVar(2);

  // Submit the DockSpace
  ImGuiIO& io = GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dockspace_id = GetID("MyDockSpace");
    DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  if (BeginMenuBar())
  {
    if (BeginMenu("Options"))
    {
      // Disabling fullscreen would allow the window to be moved to the front of other windows,
      // which we can't undo at the moment without finer window depth/z control.
      MenuItem("Fullscreen", NULL, &opt_fullscreen);
      MenuItem("Padding", NULL, &opt_padding);
      Separator();

      if (MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
      if (MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
      Separator();

      ImGui::EndMenu();
    }

    EndMenuBar();
  }
}
