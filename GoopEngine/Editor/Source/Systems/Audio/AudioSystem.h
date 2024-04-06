/*!*********************************************************************
\file   AudioSystem.h
\author c.phua\@digipen.edu
\date   8-November-2023
\brief
	Audio system.
	Updates entity's audio. Calls FmodSystems.h functions to play sounds.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
				std::string m_audio;
				float m_startVol;
				float m_endVol;
				float m_fadeStartTime;
				float m_fadeEndTime;

				float m_crossFadeTime;
				float m_currFadeTime;

				bool isOver;
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

			/*!*********************************************************************
			\brief
			  Fades in an audio
			\param fade
				The audio to fade in in the form of a CrossFade object
			************************************************************************/
			void FadeInAudio(CrossFade fade);

			/*!*********************************************************************
			\brief
				Fades out an audio
			\param fade
				The audio to fade out in the form of a CrossFade object
			************************************************************************/
			void FadeOutAudio(CrossFade fade);

    private:
			GE::fMOD::FmodSystem* m_fmodSystem;


			std::vector<CrossFade> m_fadeInList;
			std::vector<CrossFade> m_fadeOutList;
    };
	}
}