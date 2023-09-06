#include "pch.h"
#include <def.h>
#include <iostream>
#include "GLApp/Window/Window.h"
namespace windowSystem {
  Window::Window(int width, int height, char const* title) :
    m_windowWidth{ width }, m_windowHeight{ height }, m_title{ title }, m_window {} {}
  Window::~Window()
  {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
  bool Window::CreateWindow()
  {
    // Attempt to initialize GLFW
    if (!glfwInit()) {
      ERR_LOG_FILE("Failed to initialize GLFW! Exiting now ...");
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }
    // Attempt to initialize glew
    GLenum errCode{ glewInit() };
    if (errCode != GLEW_OK) {
      std::string msg{ "Failed to initialize GLEW: " };
      msg += errCode;
      ERR_LOG_FILE(msg);
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }

    // Window creation
    //m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title, nullptr, nullptr);
    //if (!m_window) {
    //  ERR_LOG_FILE("OpenGL context creation has failed! Exiting now ...");
    //  std::exit(EXIT_FAILURE); // we must leave immediately!
    //}

    return true;
  }

  void Window::MakeCurrent()
  {
    glfwMakeContextCurrent(m_window);
  }

  void Window::ErrorCallback(int error, const char* desc)
  {
    ERR_LOG(desc);
  }
}