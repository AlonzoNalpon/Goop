/*!*********************************************************************
\file   Window.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the Window class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Def.h>
#include <iostream>
#include <Window/Window.h>
namespace WindowSystem {
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
      std::cout << "Failed to initialize GLFW! Exiting now ..." << std::endl;
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }
    // Set the window hint to make the window non-resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Set the opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Window creation
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title, nullptr, nullptr);
    if (!m_window) {
      std::cout << "OpenGL context creation has failed! Exiting now ..." << std::endl;
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }
    MakeCurrent();

    // Attempt to initialize glew
    GLenum errCode{ glewInit() };
    if (errCode != GLEW_OK) {
      std::string msg{ "Failed to initialize GLEW: " };
      msg += std::to_string(errCode);
      std::cout << msg << std::endl;
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }

    glfwSetErrorCallback(ErrorCallback);       // Error callback
    glfwSetKeyCallback(m_window, KeyCallback); // key callback

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

  void Window::KeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
  {
    // For now, escape key will shut the thing down
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  void Window::ErrorCallback(int /*error*/, const char* desc)
  {
    std::cout << "Error: " << desc << std::endl; // TODO: replace this with logger
  }
}