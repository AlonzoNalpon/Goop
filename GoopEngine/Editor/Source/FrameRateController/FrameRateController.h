/*!*********************************************************************
\file   FrameRateController.h
\author han.q\@digipen.edu
\co-author w.chinkitbryan\@digipen.edu
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
#pragma once
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include "../Singleton/Singleton.h"
#include <chrono>

namespace GE
{
	namespace FPS
	{
		class FrameRateController : public Singleton<FrameRateController>
		{
		public:
			using timeFormat = std::chrono::milliseconds;
			using systemTimerList = std::vector<std::pair<std::string, timeFormat>>;

			/*!*********************************************************************
			\brief
			  Function to Initialize the Frame Rate Controller. Call this in your Main init
			\params
				targetFPS 
				FPS limit of your engine
			\params
				fpsCalInterval
				Intervals between each FPS check (seconds)
			************************************************************************/
			void InitFrameRateController(int targetFPS, int stepsPerSec = 60, int fpsCalInterval = 1);



			/*!*********************************************************************
			\brief
				Function to get the delta time
			\return
				Delta time (Amount of time pass since previous frame)
			************************************************************************/
			double GetFixedDeltaTime();


			/*!*********************************************************************
			\brief
			  Function to get the delta time
			\return
				Delta time (Amount of time pass since previous frame)
			************************************************************************/
			double GetDeltaTime() const noexcept;

			/*!*********************************************************************
			\brief
			  Function to get the current FPS of the Engine
			\return 
			  Current FPS of the Engine
			************************************************************************/
			double GetFPS() const noexcept;

			/*!*********************************************************************
			\brief
				Function to get the current target FPS of the Engine
			\return
				Current FPS of the Engine
			************************************************************************/
			double GetTargetFPS() const noexcept;

			/*!*********************************************************************
			\brief
			  Function to Signfy the end of the current frame. Call this function before the end of each game loop
			************************************************************************/
			void EndFrame();


			/*!*********************************************************************
			\brief
			  Function to Signfy the start of a new frame. call this at the start of each game loop
			************************************************************************/
			void StartFrame();


			/*!*********************************************************************
			\brief
			  Function to get the current time
			\return
			  The current time
			************************************************************************/
			double GetCurrTime() const noexcept;

			/*!*********************************************************************
			\brief
			  Function to get current frame's start time
			\return
				starting time of current frame
			************************************************************************/
			double GetStartTime() const noexcept;


			/*!*********************************************************************
			\brief
				Function to get previous frame's start time
			\return
				starting time of previous frame
			************************************************************************/
			double GetEndTime() const noexcept;


			/*!*********************************************************************
			\brief
			  Function to get the number frames passed since last check
			\return
			  Number of frames passed since the last time this function is called
			************************************************************************/
			int GetFrameCount();


			/*!*********************************************************************
			\brief
				Function to get the number of fixed delta time steps this frame
			\return
				Number of fixed delta time steps this frame
			************************************************************************/
			int GetSteps() const noexcept;


			/*!*********************************************************************
			\brief
			  Function to reset the FrameRateController
			************************************************************************/
			void ResetFrameRateController();

			
			/*!*********************************************************************
			\brief
			  Function to set the engine's FPS limit
			\params
				targetFPS
				FPS limit of your engine
			************************************************************************/
			void SetTargetFPS(int targetFPS);


			/*!*********************************************************************
			\brief
				Function to set the engine's fixed delta time target
			\params
				stepsPerSecond
				How many steps if fixed delta time per second
			************************************************************************/
			void SetStepsPerSecond(int stepsPerSecond);


			/*!*********************************************************************
			\brief
			  Function to set the engine's interval for FPS Check
			\params
				fpsCalInterval
				Intervals between each FPS check (seconds)
			************************************************************************/
			void FPSCalInterval(int fpsCalInterval);

			/*!*********************************************************************
			\brief
				Function to start the timer for system,
			************************************************************************/
			void StartSystemTimer();


			/*!*********************************************************************
		\brief
			Function to end the timer for system and record down the amount of time taken by the system
		\params
			systemName
			name of the system you want to check
		************************************************************************/
			void EndSystemTimer(std::string systemName);

			/*!*********************************************************************
			\brief
			  Getter for map of system times.
			  
			\return
			  FPS Controller system time map
			************************************************************************/
			const std::vector<std::pair<std::string, FrameRateController::timeFormat>>& GetSystemTimers() noexcept;

		private:
			double m_currentFPS{};
			double m_targetFPS{};
			double m_targetFrameTime{};
			double m_fixedDeltaTime{};
			double m_currDeltaTime{};
			double m_accumulatedTime{};
			double m_endTime{};
			double m_startTime{};
			double m_prevStartTime{};
			double m_fpsCalInterval{};
			double m_fpsCheckTime{};
			int m_currNumberOfSteps{};
			int m_frameCount{};
			int m_framePassed{};

			std::chrono::time_point<std::chrono::high_resolution_clock> m_systemTimeStart{};
			systemTimerList m_fpsControllerMap;
		};
	}
	
}