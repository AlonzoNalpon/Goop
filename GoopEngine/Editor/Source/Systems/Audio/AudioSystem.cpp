#include <pch.h>
#include <Systems/Audio/AudioSystem.h>
#include <Fmod/FmodSystem.h>
#include <FrameRateController/FrameRateController.h>

using namespace GE::ECS;
using namespace GE::Systems;
using namespace GE::Component;
using namespace GE::fMOD;

void AudioSystem::Awake()
{
  System::Awake();
  m_fmodSystem = &GE::fMOD::FmodSystem::GetInstance();
}

void AudioSystem::Update()
{
  static GE::FPS::FrameRateController& frc = GE::FPS::FrameRateController::GetInstance();

  frc.StartSystemTimer();
  std::set<Entity>& list = GetUpdatableEntities();

  for (Entity entity : list)
  {
    Audio* audio = m_ecs->GetComponent<Audio>(entity);

    for (auto& sound : audio->m_sounds)
    {
      if (sound.m_playOnStart && !sound.m_playedOnStart)
      {
        sound.m_playedOnStart = true;
        audio->Play(sound.m_sound);
      }

      if (sound.m_lastPausedState != sound.m_paused)
      {
        if (sound.m_paused)
        {
          audio->Pause(sound.m_sound);
        }
        else
        {
          audio->Play(sound.m_sound);
        }
        sound.m_lastPausedState = sound.m_paused;
      }
    }
  }
  frc.EndSystemTimer("AudioSystem");
}
