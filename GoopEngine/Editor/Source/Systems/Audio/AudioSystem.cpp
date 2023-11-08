#include <pch.h>
#include <Systems/Audio/AudioSystem.h>
#include <iostream>

using namespace GE::ECS;
using namespace GE::Systems;
using namespace GE::Component;
using namespace GE::fMOD;

AudioSystem::AudioSystem()
{
  m_fmodSystem = &FmodSystem::GetInstance();
}

void AudioSystem::Update()
{
  std::set<Entity>& list = GetUpdatableEntities();

  for (Entity entity : list)
  {
    m_audio = m_ecs->GetComponent<Audio>(entity);
    if (m_audio->m_play && !m_audio->m_isPlaying)
    {
      m_fmodSystem->PlaySound(m_audio->m_name, m_audio->m_isSFX);
      m_audio->m_isPlaying = true;
    }
    else
    {
      m_fmodSystem->StopSound(m_audio->m_name);
    }

    m_fmodSystem->SetChannelVolume(m_audio->m_isSFX, m_audio->m_volume);
  }
}