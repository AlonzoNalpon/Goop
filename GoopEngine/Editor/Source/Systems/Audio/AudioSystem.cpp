#include <pch.h>
#include <Systems/Audio/AudioSystem.h>
#include <Fmod/FmodSystem.h>
#include <iostream>

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
  std::set<Entity>& list = GetUpdatableEntities();

  for (Entity entity : list)
  {
    Audio* m_audio = m_ecs->GetComponent<Audio>(entity);

    if (!m_audio->m_initialized && (m_audio->m_playOnStart || !m_audio->m_isPlaying))
    {
      m_audio->m_initialized = true;
      m_audio->m_isPlaying = true;
      m_fmodSystem->PlaySound(m_audio->m_name, m_audio->channel, m_audio->m_loop);
    }
    else if (m_audio->m_initialized && !m_audio->m_isPlaying)
    {
      m_fmodSystem->StopSound(m_audio->m_name);
    }
  }
}