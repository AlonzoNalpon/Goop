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

    if (audio->m_playOnStart && !audio->m_playedOnStart)
    {
      audio->m_playedOnStart = true;
      audio->Play();
    }

    if (audio->m_lastPausedState != audio->m_paused)
    {
      if (audio->m_paused)
      {
        audio->Pause();
      }
      else
      {
        audio->Play();
      }
    }
  }
  frc.EndSystemTimer("AudioSystem");
}
