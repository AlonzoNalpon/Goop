#pragma once
/*!*********************************************************************
\file   InputManager.h
\author han.q\@digipen.edu
\date   17 September 2023
\brief
	Input Manager for Engine


	Setup:

	1. // Include this header file in your files
		 #include "../InputManager/InputManager.h"

	2. // Initialize the Input Manager in your Game's Init after Create Window
		 GE::Input::InputManager* im = &(GE::Input::InputManager::GetInstance());
		 im->InitInputManager(GLHelper::ptr_window, GLHelper::width, GLHelper::height);

	3. // Call theUpdateInput() function at the start of each game loop ( Start of Update function )
			GE::Input::InputManager* im = &(GE::Input::InputManager::GetInstance());
			im->UpdateInput();

 ------------------------------------------------------------------------------------------------

	How to use:

	1. Get an instance of the Input Manager in your function/code:
		 im->InitInputManager(GLHelper::ptr_window, GLHelper::width, GLHelper::height);

	2. Access the function through the instance:
		 bool isTriggered = im->IsKeyTriggered(GPK_MOUSE_LEFT);


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

//#include "../Math/GEM.h"
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>

//#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
//#define _USE_MATH_DEFINES

#include <bitset>
#include "KeyCode.h"
#include "../FrameRateController/FrameRateController.h"
#include "../Singleton/Singleton.h"
#include <array>
#include <map>
#include <vector>
#include <string>
#include <iostream>


namespace GE
{
	namespace Input
	{
		using KEY_MAP = std::bitset<static_cast<size_t>(GPK_KEY_COUNT)>;
		using KEY_PRESS_ARRAY = std::array<double, static_cast<size_t>(GPK_KEY_COUNT)>;
		using MOUSE_POS = std::pair<double, double>;

		class InputManager : public Singleton<InputManager>
		{

		private:
			//static int m_width, m_height;
			static double m_keyHeldTime;
			static MOUSE_POS m_mousePos;
			static KEY_MAP m_keyReleased;
			static KEY_MAP m_keyHeld;
			static KEY_MAP m_keysTriggered;
			static KEY_PRESS_ARRAY m_keyFramesHeld;


		public:
			/*!*********************************************************************
			\brief
				.Function to check if a key is triggered
			\params
				KEY_CODE key
				Enum of the key you want to check
			\return
				returns true is key is triggered
			************************************************************************/
			bool IsKeyTriggered(KEY_CODE);

			/*!*********************************************************************
			\brief
				.Function to check if a key is held
			\params
				KEY_CODE key
				Enum of the key you want to check
			\return
				returns true is key is held
			************************************************************************/
			bool IsKeyHeld(KEY_CODE);

			/*!*********************************************************************
			\brief
				.Function to check if a key is pressed
			\params
				KEY_CODE key
				Enum of the key you want to check
			\return
				returns true is key is pressed. (key is either triggered or held)
			************************************************************************/
			bool IsKeyPressed(KEY_CODE);

			/*!*********************************************************************
			\brief
				.Function to check if a key is released
			\params
				KEY_CODE key
				Enum of the key you want to check
			\return
				returns true is key is relased in this frame
			************************************************************************/
			bool IsKeyReleased(KEY_CODE);

			/*!*********************************************************************
			\brief
				.Function to initialize the Input Manager (Call this function right after successfully initializing window)
			\params
				GLFWwindow* window
				pointer to current window
			\params
				int width
				width of the window
			\params
				int height
				height of the window
			\params
				double holdTime
				duration a key needs to be pressed to be recognized as a held
			************************************************************************/
			void InitInputManager(GLFWwindow* window, double holdTime = 0.5);

			/*!*********************************************************************
			\brief
				.Function to update the Input for this frame. It will call GLFWPollEvents to get all key triggers and update the btiset

			************************************************************************/
			void UpdateInput();

			/*!*********************************************************************
			\brief
				.Function to get the mouse pos in the current frame

			\return
				returns the mouse's pos as dvec2
			************************************************************************/
			std::pair<double, double> GetMousePos();





			/*!*********************************************************************
		\brief
			.This callback function is called when the user triggers/release/hold a keyboard key
		\params
			GLFWwindow* window
			pointer to current window
		\params
			int key
			keyboard key that was pressed or released
		\params
			int scanCode
			scancode of the key (Platform-specific)
		\params
			int action
			action is either GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
		\params
			int mod
			bit_field representing which modifier keys (shift, alt, control) were held down
		************************************************************************/
			static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mod);

			/*!*********************************************************************
			\brief
				.This callback function is called when the user move the mouse
			\params
				GLFWwindow* window
				pointer to current window
			\params
				double xpos
				 cursor x-coordinate, relative to the left edge of the window
			\params
				double ypos
				cursor y-coordinate, relative to the left edge of the window
			************************************************************************/
			static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);

			/*!*********************************************************************
			\brief
				.This callback function is called when the user clicks a mouse button
			\params
				GLFWwindow* window
				pointer to current window
			\params
				int button
				mouse button that was pressed or released
			\params
				int action
				action is either GLFW_PRESS or GLFW_RELEASE
			\params
				int mod
				bit_field representing which modifier keys (shift, alt, control) were held down
			************************************************************************/
			static void MouseButtonCallback(GLFWwindow* pwin, int button, int action, int mod);

			/*!*********************************************************************
			\brief
				.This callback function is called when the user scrolls, whether with a mouse wheel or touchpad gesture
			\params
				GLFWwindow* window
				pointer to current window
			\params
				double
				Scroll offset along X-axis
			\params
				double yoffset
				Scroll offset along Y-axis
			************************************************************************/
			static void MouseScrollCallback(GLFWwindow* pwin, double xoffset, double yoffset);

			void TestInputManager();

		};


	}

}