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
    AppController();
    void Init();
    void Run();
    void Exit();

  private:
    WindowSystem::Window window;
    GE::EditorGUI::ImGuiUI imgui;
    Scene scn;
  };
}
