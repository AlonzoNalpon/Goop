#include "FrameRateController.h"

using namespace GE::FPS;

void FrameRateController::InitFrameRateController(int targetFPS, int fpsCalInterval)
{
	m_targetFPS = static_cast<double>(targetFPS);
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
	m_framePassed = 0;
	m_frameCount = 0;
	m_prevTime = 0.0;
	m_startTime = 0.0;
}

void FrameRateController::ResetFrameRateController()
{
	m_framePassed = 0;
	m_frameCount = 0;
	m_prevTime = 0.0;
	m_startTime = 0.0;
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

double FrameRateController::GetPrevTime() 
{
	return m_prevTime;
}

double FrameRateController::GetDeltaTime() 
{
	return m_startTime - m_prevTime;
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
}

void FrameRateController::FPSCalInterval(int fpsCalInterval)
{
	m_fpsCalInterval = static_cast<double>(fpsCalInterval);
}


void FrameRateController::EndFrame() 
{
	double targetFrameTime = 1.0 / m_targetFPS;
	while ((glfwGetTime() - m_startTime) < targetFrameTime)
	{
		//Loops until we hit the target time per frame (if the game is too fast)
	}
	m_fpsCheckTime += (glfwGetTime() - m_startTime);
	m_prevTime = m_startTime;
	++m_framePassed;
	++m_frameCount;
}


void FrameRateController::StartFrame()
{
	m_startTime = glfwGetTime();
	if (m_fpsCheckTime > m_fpsCalInterval) 
	{
		m_currentFPS = (static_cast<double>(m_framePassed) / m_fpsCheckTime);
		m_framePassed = 0;
		m_fpsCheckTime = 0.0;
	}
}

