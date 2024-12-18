/*!*********************************************************************
\file   FrameRateController.cpp
\author han.q\@digipen.edu
\date   28-September-2023
\brief
	Frame rate controller system.


	Setup:

	1. // Include this header file in your files
		 #include "../FrameRateController/FrameRateController.h"

	2. // Initialize the FPS controller in your Game's Init (After Create Window)
		 GE::FPS::FrameRateController* fps_control = &(GE::FPS::FrameRateController::GetInstance());
		 fps_control->InitFrameRateController(60,1);

	3. // Call the StartFrame() function at the start of each game loop ( Start of Update function )
		 GE::FPS::FrameRateController::GetInstance().StartFrame();

	4. // Call the EndFrame() function at the end of each game loop ( End of Draw function )
		 GE::FPS::FrameRateController::GetInstance().EndFrame();

 ------------------------------------------------------------------------------------------------

	How to use:

	1. Get an instance of the FrameRateController in your function/code:
		 GE::FPS::FrameRateController* fps = &(GE::FPS::FrameRateController::GetInstance());

	3. Access the function through the instance:
		 double dt = fps->GetDeltaTime();


Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "FrameRateController.h"
#include <EditorUI/DataViz/Visualizer.h>

using namespace GE::FPS;


void FrameRateController::InitFrameRateController(int targetFPS, int stepsPerSec, int fpsCalInterval)
{
	m_targetFPS = static_cast<double>(targetFPS);
	m_targetFrameTime = 1.0 / m_targetFPS;
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
	m_fixedDeltaTime = 1.0 / stepsPerSec;
	m_framePassed = 0;
	m_frameCount = 0;
	m_endTime = 0.0;
	m_startTime = glfwGetTime();
	m_prevStartTime = 0.0;
	m_currDeltaTime = 0.0;
	m_accumulatedTime = 0.0;
	m_timeScale = 1.f;
}

int GE::FPS::FrameRateController::GetSteps() const noexcept
{
	return m_currNumberOfSteps;
}

void FrameRateController::ResetFrameRateController()
{
	m_framePassed = 0;
	m_frameCount = 0;
	m_endTime = 0.0;
	m_startTime = 0.0;
	m_prevStartTime = 0.0;
	m_accumulatedTime = 0.0;
	m_timeScale = 1.f;
}

int FrameRateController::GetFrameCount() 
{
	int framePassed{ m_frameCount };
	m_frameCount = 0;
	return framePassed;
}


double FrameRateController::GetStartTime() const noexcept
{
	return m_startTime;
}

double FrameRateController::GetEndTime() const noexcept
{
	return m_endTime;
}

double FrameRateController::GetFixedDeltaTime()
{
	return m_fixedDeltaTime;
}

double FrameRateController::GetDeltaTime() const noexcept
{
	return m_currDeltaTime * m_timeScale;
}


double GE::FPS::FrameRateController::GetUnscaledDeltaTime() const noexcept
{
	return m_currDeltaTime;
}

double FrameRateController::GetFPS() const noexcept
{
	return m_currentFPS;
}

double FrameRateController::GetTargetFPS() const noexcept
{
	return m_targetFPS;
}

double FrameRateController::GetCurrTime() const noexcept
{
	return glfwGetTime();
}

void FrameRateController::SetTargetFPS(int targetFPS) 
{
	m_targetFPS = static_cast<double>(targetFPS);
	m_targetFrameTime = 1.0 / m_targetFPS;
}

void GE::FPS::FrameRateController::SetStepsPerSecond(int stepsPerSecond)
{
	m_fixedDeltaTime = 1.0 / stepsPerSecond;
}

void GE::FPS::FrameRateController::SetTimeScale(float scale)
{
	m_timeScale = scale;
}

void FrameRateController::FPSCalInterval(int fpsCalInterval)
{
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
}


void FrameRateController::EndFrame() 
{
	while ((glfwGetTime() - m_startTime) < m_targetFrameTime)
	{
		//Loops until we hit the target time per frame (if the game is too fast)
	}
	m_fpsCheckTime += (glfwGetTime() - m_startTime);
	m_endTime = glfwGetTime();
	++m_framePassed;
	++m_frameCount;

#ifndef IMGUI_DISABLE
	// update graph for system timers if window is shown
	EditorGUI::DataViz::Visualizer::UpdateSystemTimers();
#endif
}


void FrameRateController::StartFrame()
{
	static bool firstFrame = true;
	// First frame don't take glfw initial start time as it includes glfw start up
	// not simulation start up
	if (firstFrame)
	{
		m_startTime = m_prevStartTime = glfwGetTime();
		firstFrame = false;
	}
	else
	{
		m_prevStartTime = m_startTime;
		m_startTime = glfwGetTime();
	}
	m_currDeltaTime = m_startTime - m_prevStartTime;
	if (m_fpsCheckTime > m_fpsCalInterval) 
	{
		m_currentFPS = (static_cast<double>(m_framePassed) / m_fpsCheckTime);
		m_framePassed = 0;
		m_fpsCheckTime = 0.0;
	}
	m_currNumberOfSteps = 0;
	m_accumulatedTime += m_currDeltaTime;
	while (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_accumulatedTime -= m_fixedDeltaTime;
		++m_currNumberOfSteps;
	}	
}


void FrameRateController::StartSystemTimer()
{
	m_systemTimeStart = std::chrono::high_resolution_clock::now();
}

void FrameRateController::EndSystemTimer(std::string systemName)
{
	auto endTime = std::chrono::high_resolution_clock::now();
	systemTimerList::iterator iter = std::find_if(m_fpsControllerMap.begin(), m_fpsControllerMap.end(), [systemName](std::pair<std::string, FrameRateController::timeFormat>& timing) -> bool
	{
		return timing.first == systemName;
	});
	if (iter != m_fpsControllerMap.end())
	{
		iter->second = std::chrono::duration_cast<timeFormat>(endTime - m_systemTimeStart);
	}
	else
	{
		m_fpsControllerMap.push_back(std::make_pair(systemName, std::chrono::duration_cast<timeFormat>(endTime - m_systemTimeStart)));
	}


	m_systemTimeStart = endTime;
}


const std::vector<std::pair<std::string, FrameRateController::timeFormat>>& GE::FPS::FrameRateController::GetSystemTimers() noexcept
{
	return m_fpsControllerMap;
}
