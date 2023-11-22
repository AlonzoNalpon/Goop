/*!*********************************************************************
\file   Visualizer.h
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains functions related to functions used by the performance
        window. Currently creates a graph for each system's performance
        and the current fps of the engine. Draws data from the frame
        rate controller manager.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef NO_IMGUI
#include <vector>
#include <string>
#include <FrameRateController/FrameRateController.h>

namespace GE
{
  namespace EditorGUI::DataViz
  {
    enum class GRAPH_TYPE
    {
      HISTOGRAM,
      PROGRESS_BAR
    };
    bool operator==(int lhs, GRAPH_TYPE rhs);
    bool operator!=(int lhs, GRAPH_TYPE rhs);

    class Visualizer
    {
    public:
      //static void Init();
      /*!*********************************************************************
      \brief
        Updates the system timers graph based on data from the Frame Rate
        Controller manager.
      ************************************************************************/
      static void UpdateSystemTimers();

      /*!*********************************************************************
      \brief
        Main upate function called from ImGuiUI.cpp. Draws the entire UI of
        the window
      ************************************************************************/
      static void CreateContent(const char* tabName);
      //static void UpdateMemoryTab(const char* tabName);

      /*!*********************************************************************
      \brief
        Switches the flag to toggle the performance tab display
      ************************************************************************/
      static inline void TogglePerformanceTab() noexcept { m_performanceTabActive = !m_performanceTabActive; }
      //static inline void ToggleMemoryTab() noexcept { m_memoryTabActive = !m_memoryTabActive; }
      
      // Getters
      /*!*********************************************************************
      \brief
        Returns the flag that determines whether the performance window
        is shown
      \return
        The flag for whether the performance window is shown
      ************************************************************************/
      static inline bool IsPerformanceShown() noexcept { return m_performanceTabActive; }
      //static inline bool IsMemoryShown() noexcept { return m_memoryTabActive; }

    private:
      /*!*********************************************************************
      \brief
        Updates the graph based on the data from the frame rate controller
        manager
      ************************************************************************/
      static void UpdateGraph();

      /*!*********************************************************************
      \brief
        Updates the UI for settings to adjust the graph elements
      ************************************************************************/
      static void UpdateSettings();

      /*!*********************************************************************
      \brief
        Updates the FPS tab based on data from the frame rate controller
      ************************************************************************/
      static void UpdateFPSTab();

      /*!*********************************************************************
      \brief
        Helper function to convert the data from frame rate controller to
        float
      ************************************************************************/
      static inline float formatToFloat(GE::FPS::FrameRateController::timeFormat time) noexcept { return static_cast<float>(time.count()/* / 1e6*/); }

      static std::vector<const char*> m_systemsToGraph;
      static std::vector<float> m_systemTimers;  // corresponds to the timer for each system in m_systemsToGraph
      static std::vector<float> m_fpsHistory;

      // ImGui Elements Config
      static const float GraphColWidthPercentage, GraphRowHeightPercentage;
      static int m_graphType;
      static int m_framesPerUpdate, m_fpsMaxCount;
      static float m_maxGraphHeight, m_totalSystemTime;
      static float m_timePerUpdate;
      static bool m_performanceTabActive;
      //static bool m_memoryTabActive;
    };

  } // namespace EditorGUI
} // namespace GE
#endif