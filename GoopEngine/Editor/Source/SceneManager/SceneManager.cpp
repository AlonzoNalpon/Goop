#include "pch.h"
#include "SceneManager.h"
#include <Serialization/Serializer.h>

using namespace GE::Scenes;

void SceneManager::Init()
{
  // Load data into map
  m_nextScene = m_currentScene = "Robot";
}

void SceneManager::LoadScene()
{
  scene.Load(m_currentScene);
}

void SceneManager::InitScene()
{
  scene.Init();
}

void SceneManager::UnloadScene()
{
  scene.Unload();
}

void SceneManager::FreeScene()
{
  scene.Free();
}

void SceneManager::SetNextScene(std::string nextScene)
{
  std::string tmpScene {m_currentScene};
  UnloadScene();
  FreeScene();
  m_nextScene = m_currentScene = nextScene;
  
  try
  {
    if (nextScene == "SceneTest")
    {
      scene.TestScene();
    }
    else
    {
      LoadScene();
      InitScene();
    }
  }
  catch (std::out_of_range&)
  {
    m_nextScene = m_currentScene = tmpScene;
    if (m_currentScene == "SceneTest")
    {
      scene.TestScene();
    }
    else
    {
      LoadScene();
      InitScene();
    }
    throw Debug::Exception<SceneManager>(Debug::LEVEL_CRITICAL, ErrMsg(nextScene + ".scn doesn't exist."));
  }
}

void GE::Scenes::SceneManager::RestartScene()
{
  if (m_currentScene == "SceneTest")
  {
    UnloadScene();
    scene.TestScene();
    return;
  }
  UnloadScene();
  InitScene();
}

void GE::Scenes::SceneManager::LoadSceneFromExplorer(std::string const& filepath)
{
  std::string::size_type const startPos{ filepath.find_last_of("\\") + 1 };
  std::string const filename{ filepath.substr(startPos, filepath.find_last_of(".") - startPos) };
  // reload files if it doesn't exist
  try
  {
    am->GetScene(filename);
  }
  catch (std::out_of_range const&)
  {
    am->ReloadFiles(Assets::SCENE);
  }
  SetNextScene(filename);
}

std::string GE::Scenes::SceneManager::GetCurrentScene() const noexcept
{
  return this->m_currentScene;
}

std::string GE::Scenes::SceneManager::GetNextScene() const noexcept
{
  return this->m_nextScene;
}

void GE::Scenes::SceneManager::SaveScene() const
{
  // Save systems back to original file
  //Serialization::SerializeSystems(*Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems"));
 
  // Save the scene
  std::ostringstream filepath{};
  filepath << Assets::AssetManager::GetInstance().GetConfigData<std::string>("Assets Dir")
    << "Scenes/" << m_currentScene << Assets::AssetManager::GetInstance().GetConfigData<std::string>("Scene File Extension");
  Serialization::Serializer::GetInstance().SerializeScene(filepath.str());

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Successfully saved scene to " + filepath.str());
}
