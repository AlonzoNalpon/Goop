#include <GL/glew.h> // for access to OpenGL API declarations 
#include<GLFW/glfw3.h>
#include <iostream>

int main(int argc, char* argv[])
{
  if (!glfwInit())
  {
    // Initialization failed!
    std::cout << "Failed to initialize GLFW! Exiting now..." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::cout << "Hello World!" << std::endl;

  return 1;
}