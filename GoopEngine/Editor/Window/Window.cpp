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
  bool Window::CreateAppWindow()
  {
    // Attempt to initialize GLFW
    if (!glfwInit()) {
      ERR_LOG_FILE("Failed to initialize GLFW! Exiting now ...");
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Window creation
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title, nullptr, nullptr);
    if (!m_window) {
      ERR_LOG_FILE("OpenGL context creation has failed! Exiting now ...");
      std::exit(EXIT_FAILURE); // we must leave immediately!
    }
    MakeCurrent();

    // Attempt to initialize glew
    GLenum errCode{ glewInit() };
    if (errCode != GLEW_OK) {
      std::string msg{ "Failed to initialize GLEW: " };
      msg += std::to_string(errCode);
      ERR_LOG_FILE(msg);
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
    ERR_LOG(desc);
  }
}