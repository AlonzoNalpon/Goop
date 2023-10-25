/*!*********************************************************************
\file   ImGuiUI.h 
\author w.chinkitbryam\@digipen.edu
\date   24 September 2023
\brief  
  ImGui Editor UI Wrapper

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/EntityComponentSystem.h>
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
	private:		
		static GE::ECS::Entity m_selectedEntity;		
	public:
		/*!******************************************************************
		\brief 
		  Starts docking area

		\param[in] projectName
			Project name
		********************************************************************/
		static void CreateDockSpace(const char* projectName);

		/*!******************************************************************
		\brief 
		  Ends Docking area
		********************************************************************/
		static void EndDockSpace();

		/*!******************************************************************
		\brief 
		  Returns a read only ID of the selected entity

		\return 
			Selected entity
		********************************************************************/
		static GE::ECS::Entity GetSelectedEntity();

		/*!******************************************************************
		\brief 
		  Sets the currently selected entity

		\param[in] selectedEntity
			Entity selected
		********************************************************************/
		static void SetSelectedEntity(GE::ECS::Entity& selectedEntity);
	};
}
