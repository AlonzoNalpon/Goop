#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace WindowSystem {
  class Window {
  public:
    Window(int width, int height, char const*);
    ~Window();

    /*!*********************************************************************
    \brief
      Creates the application window.
    \params
    \return
      
    ************************************************************************/
    bool CreateAppWindow();

    /*!*********************************************************************
    \brief
      Makes the window the current context for opengl.
    \params
    \return
    ************************************************************************/
    void MakeCurrent();

    /*!*********************************************************************
    \brief
      Gets a handle to the window.
    \params
    \return
      
    ************************************************************************/
    GLFWwindow* GetWindow();

    /*!*********************************************************************
    \brief
      Returns whether window should close.
    \params
    \return
      
    ************************************************************************/
    int GetWindowShouldClose();

    /*!*********************************************************************
    \brief
      Swaps the buffers.
    \params
    \return
    ************************************************************************/
    void SwapBuffers();

    /*!*********************************************************************
    \brief
      Get the window width.
    \params
    \return
      
    ************************************************************************/
    int GetWinWidth();

    /*!*********************************************************************
    \brief
      Get the window height.
    \params
    \return
      
    ************************************************************************/
    int GetWinHeight();

    /*!*********************************************************************
    \brief Sets window title text.
    \params
      name the c string containing the new title
    \return
    ************************************************************************/
    void SetWindowTitle(char const* name);
  protected:
    /*!*********************************************************************
    \brief
      Key callback function.
    \params
      window    the window callee
      key       the key
      scancode  
      action
      mods
    \return
    ************************************************************************/
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    int m_windowWidth{}, m_windowHeight{}; //!< Dimensions of window
    const char* m_title{}; //!< title of window
    GLFWwindow* m_window{}; //!< pointer to window
  private:

    static void ErrorCallback(int error, const char* desc); //!< error callback for the window
  };
}
#endif