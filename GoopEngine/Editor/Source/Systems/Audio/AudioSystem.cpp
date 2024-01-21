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

  float dt = static_cast<float>(frc.GetDeltaTime());
  for (auto it{ m_crossFadeList.begin() }; it != m_crossFadeList.end(); ++it)
  {
    auto& cf{ *it };

    // Remove from list if done cross fading
    if (cf.m_currFadeTime >= cf.m_crossFadeTime)
    {
      m_crossFadeList.erase(it);
      // Go back one so when loop iterates it will be at the correct item
      --it;
      continue;
    }

    for (int i{ 0 }; i < 2; ++i)
    {
      // Fade audio
      if (cf.m_crossFadeStartTime[i] > cf.m_currFadeTime)
      {
        // Fade finished
        if (cf.m_crossFadeEndTime[i] > cf.m_currFadeTime)
        {
          m_fmodSystem->StopSound(cf.m_audio[i]);
        }

        // Set volume using the interpolated volume of the start, end and normalized time of current time
        m_fmodSystem->SetVolume(cf.m_audio[i], GoopUtils::Lerp(cf.m_startVol[i], cf.m_endVol[i], 
          GoopUtils::InverseLerp(cf.m_currFadeTime, cf.m_crossFadeStartTime[i], cf.m_crossFadeEndTime[i])));
      }
    }

    cf.m_currFadeTime += dt;
  }

  frc.EndSystemTimer("AudioSystem");
}

void GE::Systems::AudioSystem::CrossFadeAudio(CrossFade fade)
{
  m_crossFadeList.push_back(fade);
}
