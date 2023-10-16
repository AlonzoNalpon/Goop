#pragma once
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
      static void UpdateSystemTimes();
      static void UpdatePerformanceTab();

      static inline void TogglePerformanceTab() noexcept { m_performanceTabActive = !m_performanceTabActive; }
      static inline void ToggleMemoryTab() noexcept { m_memoryTabActive = !m_memoryTabActive; }
      
      // Getters
      static inline bool IsPerformanceShown() noexcept { return m_performanceTabActive; }
      static inline bool IsMemoryShown() noexcept { return m_memoryTabActive; }
      //static inline std::vector<const char*> const& GetSystemNames() noexcept { return m_systemsToGraph; }
      //static inline std::vector<float> const& GetSystemTimers() noexcept { return m_systemTimers; }

    private:
      static void UpdateGraph();
      static void UpdateSettings();
      static void UpdateFPSTab();

      static inline float microSecondsToFloat(std::chrono::microseconds time) noexcept { return static_cast<float>(static_cast<double>(time.count())/* / 1e6*/); }

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
      static bool m_memoryTabActive;
    };

  } // namespace EditorGUI
} // namespace GE
