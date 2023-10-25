/*!*********************************************************************
\file   InputManager.cpp
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	Input Manager for Engine


	Setup:

	1. // Include the header file in your files
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
#include <pch.h>
#include "InputManager.h"
#include <ImGui/backends/imgui_impl_glfw.h>
#include "../Events/InputEvents.h"

#define UNREFERENCED_PARAMETER(P) (P)

using namespace GE::Input;
using namespace GE::Events;

int InputManager::m_width;
double InputManager::m_scrollX;
double InputManager::m_scrollY;
int InputManager::m_height;
double InputManager::m_keyHeldTime;
vec2 InputManager::m_mousePos;
KEY_MAP InputManager::m_keyReleased;
KEY_MAP InputManager::m_keyHeld;
KEY_MAP InputManager::m_keysTriggered;
KEY_PRESS_ARRAY InputManager::m_keyFramesHeld;


void InputManager::InitInputManager(GLFWwindow* window, int width, int height, double holdTime)
{
	m_height = height;
	m_width = width;
	m_keyHeld.reset();
	m_keysTriggered.reset();
	m_keyFramesHeld.fill(0);
	m_keyHeldTime = holdTime;
	
	// Subscribe to the mouse/keyboard event
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
}


void InputManager::UpdateInput()
{
	m_keyReleased.reset();
	m_keysTriggered.reset();
	m_scrollX = m_scrollY = 0;
	glfwPollEvents();
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (int i{ 0 }; i < static_cast<int>(GPK_KEY_COUNT); ++i)
	{

		m_keyFramesHeld[i] = (m_keyReleased[i]) ? 0: (m_keyFramesHeld[i] > 0.f || m_keysTriggered[i]) ? (m_keyFramesHeld[i] < m_keyHeldTime) ? m_keyFramesHeld[i] + dt: m_keyFramesHeld[i]: 0;
		m_keyHeld[i] = (m_keyFramesHeld[i] >= m_keyHeldTime);
	}

	DispatchInputEvents();
}



void InputManager::DispatchInputEvents()
{
	Events::EventManager& eventMan{ Events::EventManager::GetInstance() };
	if (IsKeyHeld(GPK_MOUSE_LEFT))
	{
		eventMan.Dispatch(MouseHeldEvent(GPK_MOUSE_LEFT));
	}
	if (IsKeyTriggered(GPK_MOUSE_LEFT))
	{
		eventMan.Dispatch(MouseTriggeredEvent(GPK_MOUSE_LEFT));
	}
	else if (IsKeyReleased(GPK_MOUSE_LEFT))
	{
		eventMan.Dispatch(MouseReleasedEvent(GPK_MOUSE_LEFT));
	}

	if (IsKeyHeld(GPK_H))
	{
		eventMan.Dispatch(KeyHeldEvent(GPK_H));
	}
	if (IsKeyHeld(GPK_J))
	{
		eventMan.Dispatch(KeyHeldEvent(GPK_J));
	}
	if (IsKeyTriggered(GPK_K))
	{
		eventMan.Dispatch(KeyTriggeredEvent(GPK_K));
	}
	if (IsKeyTriggered(GPK_E))
	{
		eventMan.Dispatch(KeyTriggeredEvent(GPK_E));
	}
	if (IsKeyTriggered(GPK_W))
	{
		eventMan.Dispatch(KeyTriggeredEvent(GPK_W));
	}
}

bool InputManager::IsKeyTriggered(KEY_CODE key)
{
	return (m_keysTriggered[static_cast<int>(key)]);
}
bool InputManager::IsKeyHeld(KEY_CODE key)
{
	return (m_keyHeld[static_cast<int>(key)]);
}
bool InputManager::IsKeyReleased(KEY_CODE key)
{
	return (m_keyReleased[static_cast<int>(key)]);
}
bool InputManager::IsKeyPressed(KEY_CODE key)
{
	return (m_keyFramesHeld[static_cast<int>(key)] > 0.f);
}



vec2  InputManager::GetMousePos()
{
	return m_mousePos;
}


vec2  InputManager::GetMousePosWorld()
{
	vec2 worldPos = m_mousePos;
	worldPos.x -= static_cast<double>(m_width)/2;
	worldPos.y = (static_cast<double>(m_height) / 2)-worldPos.y;
	return  worldPos;
}


double InputManager::GetMouseScrollVert()
{
	return m_scrollY;
}

double InputManager::GetMouseScrollHor()
{
	return m_scrollX;
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mod)
{
	UNREFERENCED_PARAMETER(scanCode);
	UNREFERENCED_PARAMETER(mod);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
	{
		ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mod);
	}

	// returns -1 when keyboard functions such as change laptop brightness happens
	if (key < 0)
		return;

	m_keyReleased[key] = (GLFW_RELEASE == action);
	m_keysTriggered[key] = (GLFW_PRESS == action);
}

// Mouse callback function
void InputManager::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	m_mousePos.x = xpos;
	m_mousePos.y = ypos;

}

void InputManager::MouseButtonCallback(GLFWwindow* pwin, int button, int action, int mod)
{
	UNREFERENCED_PARAMETER(pwin);
	UNREFERENCED_PARAMETER(mod);

	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
	{
		ImGui_ImplGlfw_MouseButtonCallback(pwin, button, action, mod);
	}

	m_keyReleased[button] = (GLFW_RELEASE == action);
	m_keysTriggered[button] = (GLFW_PRESS == action);

}

void InputManager::MouseScrollCallback(GLFWwindow* pwin, double xoffset, double yoffset)
{
	UNREFERENCED_PARAMETER(pwin);
	m_scrollX = xoffset;
	m_scrollY = yoffset;

	ImGui_ImplGlfw_ScrollCallback(pwin, xoffset, yoffset);

	//y_off = ((y_off + yoffset) > 4) ? 4 : ((y_off + yoffset) < -4) ? -4 : y_off + yoffset;
	//std::cout << y_off << "\n";
	////#ifdef _DEBUG
	////  std::cout << "Mouse scroll wheel offset: ("
	////    << xoffset << ", " << yoffset << ")" << std::endl;
	////#endif
}


//void InputManager::TestInputManager() {
//	InputManager* im = &(GE::Input::InputManager::GetInstance());
//	if (im->IsKeyTriggered(GPK_A)) {
//		std::cout << "Key A is Triggered\n";
//	}
//	if (im->IsKeyHeld(GPK_A)) {
//		std::cout << "Key A is Held\n";
//	}
//	if (im->IsKeyReleased(GPK_A)) {
//		std::cout << "Key A is Released\n";
//	}
//
//	if (im->IsKeyPressed(GPK_MOUSE_LEFT)) {
//		std::cout << "Mouse Pos: " << im->GetMousePosWorld().x << "," << im->GetMousePosWorld().y << "\n";
//	}
//}
