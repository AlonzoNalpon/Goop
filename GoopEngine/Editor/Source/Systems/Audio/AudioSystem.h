#pragma once
#include <ECS/System/System.h>
#include <Events/EventManager.h>
#include <Component/Audio.h>
#include <Systems/Audio/FmodSystem.h>

namespace GE
{
	namespace Systems
	{
    class AudioSystem : public GE::ECS::System {
    public:
			AudioSystem();
			/*!*********************************************************************
			\brief
				Updates fmod system according to audio component within an entity.
			************************************************************************/
			void Update();

    private:
			Component::Audio* m_audio;
			fMOD::FmodSystem* m_fmodSystem;
    };
	}
}