/*!*********************************************************************
\file   FrameRateController.cpp
\author han.q\@digipen.edu
\date   28 September 2023
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


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "FrameRateController.h"

using namespace GE::FPS;


void FrameRateController::InitFrameRateController(int targetFPS, int fpsCalInterval)
{
	m_targetFPS = static_cast<double>(targetFPS);
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
	m_fixedDeltaTime = 1.0 / m_targetFPS;
	m_framePassed = 0;
	m_frameCount = 0;
	m_endTime = 0.0;
	m_startTime = glfwGetTime();
	m_prevStartTime = 0.0;
	m_currDeltaTime = 0.0;

}

void FrameRateController::ResetFrameRateController()
{
	m_framePassed = 0;
	m_frameCount = 0;
	m_endTime = 0.0;
	m_startTime = 0.0;
	m_prevStartTime = 0.0;
}

int FrameRateController::GetFrameCount() 
{
	int framePassed{ m_frameCount };
	m_frameCount = 0;
	return framePassed;
}


double FrameRateController::GetStartTime()
{
	return m_startTime;
}

double FrameRateController::GetEndTime() 
{
	return m_endTime;
}

double FrameRateController::GetFixedDeltaTime()
{
	return m_fixedDeltaTime;
}

double FrameRateController::GetDeltaTime()
{
	return m_currDeltaTime;
}


double FrameRateController::GetFPS()
{
	return m_currentFPS;
}

double FrameRateController::GetCurrTime()
{
	return glfwGetTime();
}

void FrameRateController::SetTargetFPS(int targetFPS) 
{
	m_targetFPS = static_cast<double>(targetFPS);
	m_fixedDeltaTime = 1.0 / m_targetFPS;
}

void FrameRateController::FPSCalInterval(int fpsCalInterval)
{
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
}


void FrameRateController::EndFrame() 
{
	m_fpsCheckTime += (glfwGetTime() - m_startTime);
	m_endTime = glfwGetTime();
	++m_framePassed;
	++m_frameCount;
}


void FrameRateController::StartFrame()
{
	m_prevStartTime = m_startTime;
	m_startTime = glfwGetTime();
	m_currDeltaTime = m_startTime - m_prevStartTime;
	if (m_fpsCheckTime > m_fpsCalInterval) 
	{
		m_currentFPS = (static_cast<double>(m_framePassed) / m_fpsCheckTime);
		m_framePassed = 0;
		m_fpsCheckTime = 0.0;
	}
	m_currNumberOfSteps = 0;
	m_accumulatedTime += (m_endTime - m_prevStartTime);
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
	m_fpsControllerMap[systemName] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_systemTimeStart);
	m_systemTimeStart = endTime;
}

const std::map<std::string, std::chrono::microseconds>& GE::FPS::FrameRateController::GetSystemTimers()
{
	return m_fpsControllerMap;
}
