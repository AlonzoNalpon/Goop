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

  if (!m_crossFadeList.empty())
  {
    m_crossFadeList.erase(std::remove_if(m_crossFadeList.begin(), m_crossFadeList.end(),
      [](GE::Systems::AudioSystem::CrossFade& cf)
      {
        return cf.isOver;
      }), m_crossFadeList.end());
  }

  float dt = static_cast<float>(frc.GetDeltaTime());
  for (auto it{ m_crossFadeList.begin() }; it != m_crossFadeList.end(); ++it)
  {
    auto& cf{ *it };

    for (int i{ 0 }; i < 2; ++i)
    {
      // Don't process audio that doesn't exist
      if (cf.m_audio[i] == "")
        continue;

      // First iteration of the crossfade
      if (cf.m_currFadeTime == 0.f)
      {
        // Play the track being faded into
        m_fmodSystem->PlaySound(cf.m_audio[1], cf.m_startVol[1], GE::fMOD::FmodSystem::BGM, true);
      }

      // Fade audio
      if (cf.m_currFadeTime > cf.m_crossFadeStartTime[i])
      {
        float clampedInterval = std::clamp(GoopUtils::InverseLerp(cf.m_currFadeTime, cf.m_crossFadeStartTime[i], cf.m_crossFadeEndTime[i]), 0.f, 1.f);

        // Set volume using the interpolated volume of the start, end and normalized time of current time
        m_fmodSystem->SetVolume(cf.m_audio[i], GoopUtils::Lerp(cf.m_startVol[i], cf.m_endVol[i], clampedInterval));
      }
    }

    cf.m_currFadeTime += dt;
    // Fade finished
    for (int i{ 0 }; i < 2; ++i)
    {
      if (cf.m_currFadeTime > cf.m_crossFadeTime)
      {
        // 0 is the fading out sound
        if (i == 0)
        {
          m_fmodSystem->StopSound(cf.m_audio[i]);
        }
        // 1 is fading in sound
        else if (i == 1)
        {
          // Set to target volume
          m_fmodSystem->SetVolume(cf.m_audio[i], cf.m_endVol[i]);
        }
        cf.isOver = true;
      }
    }
  }

  frc.EndSystemTimer("AudioSystem");
}

void GE::Systems::AudioSystem::CrossFadeAudio(CrossFade fade)
{
  m_crossFadeList.push_back(fade);
}
