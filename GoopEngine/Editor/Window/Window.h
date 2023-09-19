#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace WindowSystem {
  class Window {
  public:
    Window(int width, int height, char const*);
    ~Window();
    bool CreateAppWindow();
    void MakeCurrent();
    GLFWwindow* GetWindow();
    int GetWindowShouldClose();
    void SwapBuffers();

    /*!*********************************************************************
    \brief Sets window title text.
    \params
      name the c string containing the new title
    \return
    ************************************************************************/
    void SetWindowTitle(char const* name);
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