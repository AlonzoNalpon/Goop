/*!*********************************************************************
\file   Window.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the Window class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Def.h>
#include <iostream>
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>

namespace WindowSystem 
{

  Window::Window(int width, int height, char const* title) :
    m_windowWidth{ width }, m_windowHeight{ height }, m_title{ title }, m_window {} {}

  Window::~Window()
  {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  Window& Window::operator=(const Window* other)
  {
    // Copy the value from the other object
    this->m_windowWidth = other->m_windowWidth;
    this->m_windowHeight = other->m_windowHeight;
    this->m_title = other->m_title;
    this->m_window = other->m_window;
    return *this;
  }

  bool Window::CreateAppWindow()
  {
    // Attempt to initialize GLFW
    if (!glfwInit()) {
      throw GE::Debug::Exception<Window>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Failed to initialize GLFW!"));
    }

    GE::Assets::AssetManager& am { GE::Assets::AssetManager::GetInstance() };
    bool shouldFullScreen = am.GetConfigData<bool>("Fullscreen");
    if (shouldFullScreen)
    {
      // Get window width and height
      GetMonitorDimensions(m_windowWidth, m_windowHeight);
    }
    else
    {
      m_windowWidth = am.GetConfigData<int>("Window Width");
      m_windowHeight = am.GetConfigData<int>("Window Height");
    }

    // Set the window hint to make the window non-resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Set the opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Window creation
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title, nullptr, nullptr);
    if (!m_window) {
      throw GE::Debug::Exception<Window>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("OpenGL context creation has failed!"));
    }

    MakeCurrent();

    // Attempt to initialize glew
    GLenum errCode{ glewInit() };
    if (errCode != GLEW_OK) {
      throw GE::Debug::Exception<Window>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Failed to initialize GLEW: " + std::to_string(errCode)));
    }
    glfwSetWindowFocusCallback(m_window, WindowFocusedCallback);
    glfwSetErrorCallback(ErrorCallback);       // Error callback
    glfwSetKeyCallback(m_window, KeyCallback); // key callback

    if (shouldFullScreen)
    {
      ToggleFullscreen(); // Switch to fullscreen
    }
    return true;
  }

  void Window::MakeCurrent()
  {
    glfwMakeContextCurrent(m_window);
  }

  GLFWwindow* Window::GetWindow()
  {
    return m_window;
  }

  int Window::GetWindowShouldClose()
  {
    return glfwWindowShouldClose(m_window);
  }

  void Window::SwapBuffers()
  {
    glfwSwapBuffers(m_window);
  }

  int Window::GetWinWidth()
  {
      return m_windowWidth;
  }

  int Window::GetWinHeight()
  {
    return m_windowHeight;
  }

  void Window::SetWindowTitle(char const* name)
  {
    glfwSetWindowTitle(m_window, name);
  }

  bool Window::IsFocused()
  {
      return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
  }

  bool Window::IsFullscreen()
  {
    return glfwGetWindowMonitor(m_window) != nullptr;
  }

  void Window::ToggleFullscreen()
  {
    if (IsFullscreen())
    {
      // Switch to windowed mode
      glfwSetWindowMonitor(m_window, nullptr, m_windowXPos, m_windowYPos, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
      GE::Input::InputManager::GetInstance().SetDim(m_windowWidth, m_windowHeight);
      GE::Graphics::GraphicsEngine::GetInstance().SetDim(m_windowWidth, m_windowHeight);
    }
    else
    {
      glfwGetWindowPos(m_window, &m_windowXPos, &m_windowYPos);
      // Switch to fullscreen mode
      int monitorCount{};
      auto** monitors = glfwGetMonitors(&monitorCount);
      auto* currMonitor = monitors[0];
      const GLFWvidmode* mode = glfwGetVideoMode(currMonitor);
      glfwSetWindowMonitor(m_window, currMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
      GE::Input::InputManager::GetInstance().SetDim(mode->width, mode->height);
      GE::Graphics::GraphicsEngine::GetInstance().SetDim(mode->width, mode->height);
    }
  }

  void Window::MinimizeWindow()
  {
    glfwIconifyWindow(m_window);
  }

  void Window::GetMonitorDimensions(int& width, int& height)
  {
    int monitorCount{};
    auto** monitors = glfwGetMonitors(&monitorCount);
    auto* currMonitor = monitors[0];
    const GLFWvidmode* mode = glfwGetVideoMode(currMonitor);
    width = mode->width;
    height = mode->height;
  }

  void Window::KeyCallback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
  {

  }

  void Window::WindowFocusedCallback(GLFWwindow* /*window*/, int focused)
  {
    if (!focused)
    {
      GE::Events::EventManager::GetInstance().Dispatch(GE::Events::WindowLoseFocusEvent());
      
      if (GE::Input::InputManager::GetInstance().IsKeyPressed(GPK_LEFT_ALT))
        GE::Events::EventManager::GetInstance().Dispatch(GE::Events::WindowMinimize());
    }
    else
    {
      GE::Events::EventManager::GetInstance().Dispatch(GE::Events::WindowGainFocusEvent());
    }
  }

  void Window::ErrorCallback(int /*error*/, const char* desc)
  {
    throw GE::Debug::Exception<Window>(GE::Debug::LEVEL_INFO,
      ErrMsg(std::string{ "Info: " } + desc));
  }
}