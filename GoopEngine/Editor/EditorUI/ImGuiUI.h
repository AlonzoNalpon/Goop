/*!******************************************************************
\file   ImGuiUI.h 
\author w.chinkitbryam\@digipen.edu
\date   24 September 2023
\brief  
  ImGui Editor UI Wrapper
********************************************************************/
#pragma once
#include <pch.h>
#include <Window/Window.h>

namespace GE::EditorGUI
{
	class ImGuiUI
	{
	private:
		WindowSystem::Window* window;
		GE::ECS::EntityComponentSystem* ecs;
	public:
		/*!******************************************************************
		\brief 
		  Initializes ImGui to glfw window and opengl

		\param[in] window
			Pointer to glfw window
		********************************************************************/
		void Init(WindowSystem::Window& prgmWindow);

		/*!******************************************************************
		\brief 
		  Editor GUI logic is done here
		********************************************************************/
		void Update();

		/*!******************************************************************
		\brief 
		  Renders GUI
		********************************************************************/
		void Render();

		/*!******************************************************************
		\brief 
		  Properly exits ImGui
		********************************************************************/
		void Exit();
	};

	class ImGuiHelper
	{
	public:
		void static CreateDockSpace(const char* projectName);
		void static EndDockSpace();
	};
}
