//#include <GLFW/glfw3.h>
#include <iostream>
#include "pch.h"
#include "GLApp/Window/Window.h"
int main(int argc, char* argv[])
{
  //if (!glfwInit())
  //{
  //  // Initialization failed!
  //  std::cout << "Failed to initialize GLFW! Exiting now..." << std::endl;
  //  std::exit(EXIT_FAILURE);
  //}

  //Goop::Window window(100, 100, L"WHAT");
  //window.CreateOGLWindow();
  windowSystem::Window window{ 640, 480, "GOOP"};
  window.CreateWindow();

  return 1;
}