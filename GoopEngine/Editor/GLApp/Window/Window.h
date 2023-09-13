#ifndef WINDOW_H
#define WINDOW_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace windowSystem {
  class Window {
  public:
    Window(int width, int height, char const*);
    ~Window();
    bool CreateWindow();
    void MakeCurrent();
    GLFWwindow* GetWindow();
    int GetWindowShouldClose();
    void SwapBuffers();
  protected:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    int m_windowWidth{}, m_windowHeight{}; //!< Dimensions of window
    const char* m_title{}; //!< title of window
    GLFWwindow* m_window{}; //!< pointer to window
  private:
    static void ErrorCallback(int error, const char* desc);
  };
}
#endif