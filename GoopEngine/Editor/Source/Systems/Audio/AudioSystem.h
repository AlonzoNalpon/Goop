/*!*********************************************************************
\file   AudioSystem.h
\author c.phua\@digipen.edu
\date   8 November 2023
\brief
	Audio system.
	Updates entity's audio. Calls FmodSystems.h functions to play sounds.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/System/System.h>
#include <Events/EventManager.h>
#include <Component/Audio.h>
#include <Fmod/FmodSystem.h>

namespace GE
{
	namespace Systems
	{
    class AudioSystem : public GE::ECS::System 
		{
    public:
			struct CrossFade
			{				
				std::string m_audio[2];
				float m_startVol[2];
				float m_endVol[2];
				float m_crossFadeStartTime[2];
				float m_crossFadeEndTime[2];

				float m_crossFadeTime;
				float m_currFadeTime;
			};

			/*!*********************************************************************
			\brief
			  Initializes fmodsystem
			************************************************************************/
			void Awake();

			/*!*********************************************************************
			\brief
				Updates fmod system according to audio component within an entity.
			************************************************************************/
			void Update();

			void CrossFadeAudio(CrossFade fade);

    private:
			GE::fMOD::FmodSystem* m_fmodSystem;


			std::vector<CrossFade> m_crossFadeList;
    };
	}
}