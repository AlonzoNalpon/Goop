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

  if (!m_fadeInList.empty())
  {
    m_fadeInList.erase(std::remove_if(m_fadeInList.begin(), m_fadeInList.end(),
      [](GE::Systems::AudioSystem::CrossFade& cf)
      {
        return cf.isOver;
      }), m_fadeInList.end());
  }
  if (!m_fadeOutList.empty())
  {
    m_fadeOutList.erase(std::remove_if(m_fadeOutList.begin(), m_fadeOutList.end(),
      [](GE::Systems::AudioSystem::CrossFade& cf)
      {
        return cf.isOver;
      }), m_fadeOutList.end());
  }

  float dt = static_cast<float>(frc.GetUnscaledDeltaTime());
  for (auto it{ m_fadeInList.begin() }; it != m_fadeInList.end(); ++it)
  {
    auto& cf{ *it };

    // First iteration of the crossfade
    if (cf.m_currFadeTime == 0.f)
    {
      // Play the track being faded into
      m_fmodSystem->PlaySound(cf.m_audio, cf.m_startVol, GE::fMOD::FmodSystem::BGM, true);
    }

    // Fade audio
    if (cf.m_currFadeTime > cf.m_fadeStartTime)
    {
      float clampedInterval = std::clamp(GoopUtils::InverseLerp(cf.m_currFadeTime, cf.m_fadeStartTime, cf.m_fadeEndTime), 0.f, 1.f);

      // Set volume using the interpolated volume of the start, end and normalized time of current time
      m_fmodSystem->SetVolume(cf.m_audio, GoopUtils::Lerp(cf.m_startVol, cf.m_endVol, clampedInterval));
    }

    cf.m_currFadeTime += dt;
    // Fade finished
    if (cf.m_currFadeTime > cf.m_crossFadeTime)
    {
      m_fmodSystem->SetVolume(cf.m_audio, cf.m_endVol);
      cf.isOver = true;
    }
  }

  for (auto it{ m_fadeOutList.begin() }; it != m_fadeOutList.end(); ++it)
  {
    auto& cf{ *it };

    // Fade audio
    if (cf.m_currFadeTime > cf.m_fadeStartTime)
    {
      float clampedInterval = std::clamp(GoopUtils::InverseLerp(cf.m_currFadeTime, cf.m_fadeStartTime, cf.m_fadeEndTime), 0.f, 1.f);

      // Set volume using the interpolated volume of the start, end and normalized time of current time
      m_fmodSystem->SetVolume(cf.m_audio, GoopUtils::Lerp(cf.m_startVol, cf.m_endVol, clampedInterval));
    }

    cf.m_currFadeTime += dt;
    // Fade finished
    if (cf.m_currFadeTime > cf.m_crossFadeTime)
    {
      m_fmodSystem->StopSound(cf.m_audio);
      cf.isOver = true;
    }
  }

  frc.EndSystemTimer("AudioSystem");
}

void GE::Systems::AudioSystem::FadeInAudio(CrossFade fade)
{
  static auto& fmod = GE::fMOD::FmodSystem::GetInstance();
  // Don't fade if already playing
  if (fmod.isPlaying(fade.m_audio))
    return;

  m_fadeInList.push_back(fade);
}
void GE::Systems::AudioSystem::FadeOutAudio(CrossFade fade)
{
  static auto& fmod = GE::fMOD::FmodSystem::GetInstance();
  // Don't fade if not playing
  if (!fmod.isPlaying(fade.m_audio))
    return;

  fade.m_startVol = fmod.GetVolume(fade.m_audio);
  m_fadeOutList.push_back(fade);
}
