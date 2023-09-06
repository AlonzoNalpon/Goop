#include "pch.h"
#include <def.h>
#include <iostream>
#include "GLApp/Window/Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace windowSystem {
  Window::Window()
  {

  }
  Window::~Window()
  {

  }
  bool Window::CreateWindow()
  {
    if (!glfwInit()) {
      ERR_LOG_FILE("Failed to initialize GLFW! Exiting now");
      std::exit(EXIT_FAILURE);
      return false;
    }
    ERR_LOG_FILE("THIS IS GOOD");
    return true;
  }
}