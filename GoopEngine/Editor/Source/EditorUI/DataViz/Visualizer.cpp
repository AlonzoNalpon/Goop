#include <pch.h>
#include "Visualizer.h"
#include <ImGui/imgui.h>
#include <MemoryManager/MemoryManager.h>
#include <numeric>

using namespace GE::EditorGUI;
using namespace DataViz;
using namespace ImGui;

const float Visualizer::GraphColWidthPercentage{ 0.75f }, 
            Visualizer::GraphRowHeightPercentage{ 0.55f };

int Visualizer::m_graphType{ static_cast<int>(GRAPH_TYPE::HISTOGRAM) };
int Visualizer::m_framesPerUpdate{ 5 }, Visualizer::m_fpsMaxCount{ 10 };
float Visualizer::m_maxGraphHeight{ 2000.f }, Visualizer::m_totalSystemTime{};
float Visualizer::m_timePerUpdate{ 0.5f };
bool Visualizer::m_performanceTabActive{ true };// , Visualizer::m_memoryTabActive{ true };
std::vector<const char*> Visualizer::m_systemsToGraph;
std::vector<float> Visualizer::m_systemTimers;
std::vector<float> Visualizer::m_fpsHistory(Visualizer::m_fpsMaxCount, 0.f);

//void Visualizer::Init()
//{
//  m_graphWidth = 
//}

void Visualizer::UpdateSystemTimers()
{
  if (IsPerformanceShown())
  {
    GE::FPS::FrameRateController& fRC{ FPS::FrameRateController::GetInstance() };

    // Systems
    static int framesElapsed{};
    if (framesElapsed >= m_framesPerUpdate)
    {
      auto const& timers{ fRC.GetSystemTimers() };
      // clear and reset maps if size changes
      if (m_systemsToGraph.size() != timers.size())
      {
        m_systemsToGraph.clear();
        m_systemTimers.clear();
        m_systemsToGraph.resize(timers.size());
        m_systemTimers.resize(timers.size());
      }

      m_totalSystemTime = 0.f;
      auto iter{ timers.cbegin() };
      for (unsigned i{}; i < timers.size(); ++i, ++iter)
      {
        m_systemsToGraph[i] = iter->first.c_str();
        m_totalSystemTime += m_systemTimers[i] = microSecondsToFloat(iter->second);
      }
      framesElapsed = 0;
    }
    ++framesElapsed;

    // FPS
    static double timeElapsed{};
    if (static_cast<float>(timeElapsed) >= m_timePerUpdate)
    {
      if (m_fpsHistory.size() >= m_fpsMaxCount)
      {
        m_fpsHistory.erase(m_fpsHistory.begin());
      }
      m_fpsHistory.emplace_back(static_cast<float>(fRC.GetFPS()));
      timeElapsed = 0.f;
    }
    timeElapsed += fRC.GetDeltaTime();
  }
}

void Visualizer::CreateContent(const char* tabName)
{
  ImGui::Begin(tabName);

  float const graphWidth{ GetWindowWidth() * GraphColWidthPercentage },
    graphHeight{ GetWindowHeight() * GraphRowHeightPercentage };
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, graphWidth);
  ImGui::SetColumnWidth(1, GetWindowWidth() - graphWidth);

  ImGui::BeginChild("Graph", ImVec2(graphWidth, graphHeight), false, ImGuiWindowFlags_NoResize);
  {
    UpdateGraph();
  }
  ImGui::EndChild();
  ImGui::NextColumn();
  ImGui::BeginChild("Settings", ImVec2(GetWindowWidth() - graphWidth, graphHeight), false, ImGuiWindowFlags_NoResize);
  {
    UpdateSettings();
  }
  ImGui::EndChild();
  ImGui::Columns(1);
  ImGui::Separator();
  ImGui::BeginChild("FPS", ImVec2(GetWindowWidth(), GetWindowHeight() - graphHeight), false, ImGuiWindowFlags_NoResize);
  {
    UpdateFPSTab();
  }
  ImGui::EndChild();
  ImGui::End();
}

void Visualizer::UpdateGraph()
{
  float const graphWidth{ GetColumnWidth() }, graphHeight{ GetWindowHeight() * GraphRowHeightPercentage };
  float const averageSysTime{ m_totalSystemTime / static_cast<float>(m_systemTimers.size()) };

  if (m_graphType == GRAPH_TYPE::HISTOGRAM)
  {
    std::string const averageStr{ "Average: " + std::to_string(m_totalSystemTime / static_cast<float>(m_systemTimers.size())) + "us" };
    ImGui::PlotHistogram("##PerformanceHistogram", m_systemTimers.data(), static_cast<int>(m_systemTimers.size()), 
      0, averageStr.c_str(), 0.f, m_maxGraphHeight, ImVec2(graphWidth, graphHeight));
    
    // convert coords from histogram to window
    ImVec2 const rectMin{ GetItemRectMin() };
    float const averageYPos{ rectMin.y + (1.f - averageSysTime / m_maxGraphHeight) * (GetItemRectSize().y)};
    ImVec2 const lineStart(rectMin.x, averageYPos), lineEnd(GetItemRectMax().x, averageYPos);

    if (m_systemTimers.empty())
      return;

    ImGui::Columns(static_cast<int>(m_systemTimers.size()));
    // Render names below histogram
    for (int i = 0; i < static_cast<int>(m_systemTimers.size()); ++i)
    {
      float percentage{ 100.f * m_systemTimers[i] / m_totalSystemTime };
      ImGui::TextWrapped("%s\n%.fus (%.2f%%)", m_systemsToGraph[i], m_systemTimers[i], percentage, graphWidth / static_cast<float>(m_systemTimers.size()));
      ImGui::NextColumn();
    }
    ImGui::Columns(1);

    // Add the average line
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 const lineColor = ImGui::GetStyleColorVec4(ImGuiCol_PlotLines);
    drawList->AddLine(lineStart, lineEnd,
      IM_COL32(lineColor.x * 255, lineColor.y * 255, lineColor.z * 255, lineColor.w * 255),
      1.f);
  }
  else  // Progress Bar display mode
  {
    float const innerLeftColWidth{ graphWidth * 0.15f };
    float const graphRectWidth{ GetItemRectSize().x * 0.85f };

    // convert coords from histogram to window
    float const averageXPos{ GetItemRectMax().x - graphRectWidth + (averageSysTime / m_totalSystemTime) * graphRectWidth };
    for (int i = 0; i < static_cast<int>(m_systemTimers.size()); ++i)
    {
      // for each system, display text on 1st col and bar on 2nd col
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, innerLeftColWidth);
      ImGui::SetColumnWidth(1, graphWidth - innerLeftColWidth);
      ImGui::TextWrapped("%s (%.fus)", m_systemsToGraph[i], m_systemTimers[i], (graphWidth, graphHeight / static_cast<float>(m_systemTimers.size())));
      ImGui::NextColumn();
      ImGui::ProgressBar(m_systemTimers[i] / m_totalSystemTime, ImVec2(graphWidth, graphHeight / static_cast<float>(m_systemTimers.size())));
      ImGui::Columns(1);
    }

    // Add the average line
    ImVec2 const lineStart(averageXPos, 0), lineEnd(averageXPos, GetItemRectMax().y);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec4 const lineColor = ImGui::GetStyleColorVec4(ImGuiCol_PlotLines);
    drawList->AddLine(lineStart, lineEnd,
      IM_COL32(lineColor.x * 255, lineColor.y * 255, lineColor.z * 255, lineColor.w * 255),
      1.f);
  }
}

void Visualizer::UpdateSettings()
{
  ImGui::Text("Graph Type");
  if (ImGui::Combo("##GraphType##Selector", &m_graphType, "Histogram\0Progress Bar\0"))
  {
    if (m_graphType == GRAPH_TYPE::HISTOGRAM)
    {
      m_graphType = static_cast<int>(GRAPH_TYPE::HISTOGRAM);
    }
    else if (m_graphType == GRAPH_TYPE::PROGRESS_BAR)
    {
      m_graphType = static_cast<int>(GRAPH_TYPE::PROGRESS_BAR);
    }
  }

  ImGui::Text("No. of Frames per Update");
  ImGui::SliderInt("##FramesPerUpdateSlider", &m_framesPerUpdate, 1, 300);

  if (m_graphType == GRAPH_TYPE::HISTOGRAM)
  {
    ImGui::Text("Graph Max Height");
    ImGui::SliderFloat("##GraphMaxHeightSlider", &m_maxGraphHeight, 1000.0f, 2500.0f);
  }
}

void Visualizer::UpdateFPSTab()
{
  float const graphWidth{ GetWindowWidth() * GraphColWidthPercentage };
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, graphWidth);
  ImGui::SetColumnWidth(1, GetWindowWidth() - graphWidth);

  const float sum{ std::accumulate(m_fpsHistory.begin(), m_fpsHistory.end(), 0.f) };
  std::string const averageStr{ "Average: " + std::to_string(sum / static_cast<float>(m_fpsHistory.size())) };

  ImGui::Text("FPS Chart");
  ImGui::SameLine();
  ImGui::PlotLines("##FPSChart", m_fpsHistory.data(), m_fpsMaxCount,
    0, averageStr.c_str(), 0.f, static_cast<float>(FPS::FrameRateController::GetInstance().GetTargetFPS()) + 10.f, ImVec2(GetColumnWidth(), GetWindowHeight() * 0.7f));

  ImGui::NextColumn();

  ImGui::Text("Time per Update");
  ImGui::SliderFloat("##TimePerUpdate", &m_timePerUpdate, 0.1f, 5.f);

  ImGui::Text("Width");
  if (ImGui::SliderInt("##FPSMaxCount", &m_fpsMaxCount, 2, 50))
  {
    if (m_fpsMaxCount > m_fpsHistory.size())
    {
      m_fpsHistory.resize(m_fpsMaxCount);
    }
    else if (m_fpsMaxCount < m_fpsHistory.size())
    {
      m_fpsHistory.erase(m_fpsHistory.begin(), m_fpsHistory.end() - m_fpsMaxCount);
    }
  }
}

//void Visualizer::UpdateMemoryTab(const char* tabName)
//{
//  Memory::MemoryManager::GetInstance().
//
//  ImGui::Begin(tabName);
//  ImGui::ProgressBar()
//  ImGui::End();
//}

bool GE::EditorGUI::DataViz::operator==(int lhs, GRAPH_TYPE rhs)
{
  return lhs == static_cast<int>(rhs);
}
bool GE::EditorGUI::DataViz::operator!=(int lhs, GRAPH_TYPE rhs)
{
  return lhs != static_cast<int>(rhs);
}