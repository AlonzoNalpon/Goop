/*!*********************************************************************
\file   FrameRateController.h
\author han.q@digipen.edu
\date   $DAY_OF_MONTH-$MONTHNAME_EN-$YEAR
\brief
	Frame rate controller system.
	Instructions:

	1. Include this header file in your file
		 #include "../FrameRateController/FrameRateController.h"

	2. Get an instance of the FrameRateController in your function/code:
	   GE::FPS::FrameRateController* fps = &(GE::FPS::FrameRateController::GetInstance());

	3. Access the function through the instance:
	   double dt = fps->GetDeltaTime();

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include "../Singleton/Singleton.h"

namespace GE
{
	namespace FPS
	{
		class FrameRateController : public Singleton<FrameRateController>
		{

			int m_frameCount{};
			int m_framePassed{};
			double m_currentFPS{};
			double m_targetFPS{};
			double m_prevTime{};
			double m_startTime{};
			double m_fpsCalInterval{};
			double m_fpsCheckTime{};

			

		public:
			void InitFrameRateController(int targetFPS, int fpsCalInterval);
			void ResetFrameRateController();
			int GetFrameCount();
			double GetStartTime();
			double GetPrevTime();
			double GetDeltaTime();
			double GetCurrTime();
			double GetFPS();
			void EndFrame();
			void StartFrame();
			void SetTargetFPS(int targetFPS);
			void FPSCalInterval(int fpsCalInterval);

		};
	}
	
}