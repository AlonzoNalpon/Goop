#include "InputManager.h"
#include <imgui.h>
#include <ImGui/backends/imgui_impl_glfw.h>

#define UNREFERENCED_PARAMETER(P) (P)

using namespace GE::Input;

//Wint InputManager::m_width, InputManager::m_height;
double InputManager::m_keyHeldTime;
MOUSE_POS InputManager::m_mousePos;
KEY_MAP InputManager::m_keyReleased;
KEY_MAP InputManager::m_keyHeld;
KEY_MAP InputManager::m_keysTriggered;
KEY_PRESS_ARRAY InputManager::m_keyFramesHeld;


void InputManager::InitInputManager(GLFWwindow* window, double holdTime)
{
	//m_height = height;
	//m_width = width;
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
	glfwPollEvents();
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (int i{ 0 }; i < static_cast<int>(GPK_KEY_COUNT); ++i)
	{
		m_keyFramesHeld[i] = (m_keysTriggered[i]) ? (m_keyFramesHeld[i] < m_keyHeldTime) ? m_keyFramesHeld[i] + dt : m_keyFramesHeld[i] : 0;
		m_keyHeld[i] = (m_keyFramesHeld[i] >= m_keyHeldTime);
	}

}

bool InputManager::IsKeyTriggered(KEY_CODE key)
{
	return (!m_keyHeld[static_cast<int>(key)] && m_keysTriggered[static_cast<int>(key)]);
}
bool InputManager::IsKeyHeld(KEY_CODE key)
{
	return (m_keyHeld[static_cast<int>(key)]);
}

bool InputManager::IsKeyPressed(KEY_CODE key)
{
	return (m_keyHeld[static_cast<int>(key)] || m_keysTriggered[static_cast<int>(key)]);
}

bool InputManager::IsKeyReleased(KEY_CODE key)
{
	return (m_keyReleased[static_cast<int>(key)]);
}

MOUSE_POS  InputManager::GetMousePos()
{
	return m_mousePos;
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

	bool bit = !(GLFW_RELEASE == action);
	m_keyReleased[key] = !bit;
	m_keysTriggered[key] = bit;
}

// Mouse callback function
void InputManager::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	m_mousePos.first = xpos;
	m_mousePos.second = ypos;

}

void InputManager::MouseButtonCallback(GLFWwindow* pwin, int button, int action, int mod)
{
	UNREFERENCED_PARAMETER(pwin);
	UNREFERENCED_PARAMETER(mod);

	ImGuiIO& io = ImGui::GetIO();
	bool bit = !(GLFW_RELEASE == action);

	if (io.WantCaptureMouse)
	{
		ImGui_ImplGlfw_MouseButtonCallback(pwin, button, action, mod);
	}

	m_keyReleased[button] = !bit;
	m_keysTriggered[button] = bit;

}

void InputManager::MouseScrollCallback(GLFWwindow* pwin, double xoffset, double yoffset)
{
	UNREFERENCED_PARAMETER(pwin);
	UNREFERENCED_PARAMETER(xoffset);
	UNREFERENCED_PARAMETER(yoffset);

	ImGui_ImplGlfw_ScrollCallback(pwin, xoffset, yoffset);

	//y_off = ((y_off + yoffset) > 4) ? 4 : ((y_off + yoffset) < -4) ? -4 : y_off + yoffset;
	//std::cout << y_off << "\n";
	////#ifdef _DEBUG
	////  std::cout << "Mouse scroll wheel offset: ("
	////    << xoffset << ", " << yoffset << ")" << std::endl;
	////#endif
}


void InputManager::TestInputManager() {
	InputManager* im = &(GE::Input::InputManager::GetInstance());
	if (im->IsKeyTriggered(GPK_A)) {
		std::cout << "Key A is Triggered\n";
	}
	if (im->IsKeyHeld(GPK_A)) {
		std::cout << "Key A is Held\n";
	}
	if (im->IsKeyReleased(GPK_A)) {
		std::cout << "Key A is Released\n";
	}

	if (im->IsKeyPressed(GPK_MOUSE_LEFT)) {
		std::cout << "Mouse Pos: " << im->GetMousePos().first << "," << im->GetMousePos().second << "\n";
	}
}
