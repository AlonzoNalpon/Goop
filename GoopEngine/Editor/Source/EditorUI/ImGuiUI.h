/*!*********************************************************************
\file   ImGuiUI.h 
\author w.chinkitbryam\@digipen.edu
\date   24 September 2023
\brief  
  ImGui Editor UI Wrapper

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include <ECS/EntityComponentSystem.h>
#include <Window/Window.h>
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace GE::EditorGUI
{
	class ImGuiUI
	{
	private:
		WindowSystem::Window* window;
		GE::ECS::EntityComponentSystem* ecs;
		std::pair<Graphics::gObjID, Graphics::Rendering::FrameBufferInfo*> frameBuffer;
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
		static std::string m_selectedAsset;
		static bool m_play;
		static bool m_pause;
		static bool m_step;
		static bool m_restart;
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

		/*!******************************************************************
		\brief
			Returns a read only ID of the selected asset

		\return
			Selected asset
		********************************************************************/
		static std::string GetSelectedAsset();

		/*!******************************************************************
		\brief
			Sets the currently selected asset

		\param[in] selectedEntity
			Entity asset
		********************************************************************/
		static void SetSelectedAsset(std::string selectedAssetPath);

		/*!*********************************************************************
		\brief
		  Sets the flag if you should step simulation.

		\param shouldStep
		  Flag
		************************************************************************/
		static void StepSimulation(bool shouldStep);

		/*!*********************************************************************
		\brief
		  Returns the value of the flag set by StepSimulation. Then sets the
			flag to false.
		  
		\return
		  Step flag
		************************************************************************/
		static bool StepSimulation();

		/*!*********************************************************************
		\brief
		  Get the flag is simulation should be running.
		  
		\return
		  ShouldPlay flag
		************************************************************************/
		static bool ShouldPlay();

		/*!*********************************************************************
		\brief
		  Sets the should play flag for the simulation to true.
		************************************************************************/
		static void Play();

		/*!*********************************************************************
		\brief
		  Sets the should play flag for the simulation to false.	
		************************************************************************/
		static void Pause();

		/*!*********************************************************************
		\brief
		  Returns the flag of if the simulation is paused.
		  
		\return
		  Paused flag
		************************************************************************/
		static bool Paused();

		/*!*********************************************************************
		\brief
		  Sets the should restart flag for the simulation to true.		
		************************************************************************/
		static void Restart();

		/*!*********************************************************************
		\brief
		  Returns the flag of if the simulation is playing.
		  
		\return
		  Play flag
		************************************************************************/
		static bool IsRunning();

		/*!*********************************************************************
		\brief
		  Returns the flag of if the simulation should be restarted. The flag
			will be set to false after the function is called.
		  
		\return
		  Restart flag
		************************************************************************/
		static bool ShouldRestart();

	};
}
#endif