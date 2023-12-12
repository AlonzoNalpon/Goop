#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace WindowSystem {
  class Window {
  public:
    Window(int width, int height, char const*);
    ~Window();
    Window& operator=(const Window* other);

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

    /*!*********************************************************************
    \brief
      Checks if window is focused.
    \params
    \return
      
    ************************************************************************/
    bool IsFocused();

    /*!*********************************************************************
    \brief
      Checks if a window is fullscreen.
    \params
    \return
      
    ************************************************************************/
    bool IsFullscreen();

    /*!*********************************************************************
    \brief
      Toggles fullscreen. Takes note of window position automatically.
    \params
    \return
    ************************************************************************/
    void ToggleFullscreen();

    /*!*********************************************************************
    \brief
      Minimizes the window. This function does nothing if window is already
      minimized.
    \params
    \return
    ************************************************************************/
    void MinimizeWindow();

    /*!*********************************************************************
    \brief
      Gets the monitor resolution. Typically used for setting resolution.
    \params
      width
      height
    \return
    ************************************************************************/
    static void GetMonitorDimensions(int& width, int& height);
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
    int m_windowXPos{}, m_windowYPos{}; //!< window position (only for toggling fullscreen)
    const char* m_title{}; //!< title of window
    GLFWwindow* m_window{}; //!< pointer to window
  private:
    static void WindowFocusedCallback(GLFWwindow* window, int focused);
    static void ErrorCallback(int error, const char* desc); //!< error callback for the window
  };
}
#endif