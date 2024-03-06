/*!*********************************************************************
\file   GameStateManager.cpp
\author loh.j\@digipen.edu
\date   24-October-2023
\brief
	Currently controls the scenes but will include gamestates in the 
	future.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <GameStateManager/GameStateManager.h>
#include <EditorUI/ImGuiUI.h>
using namespace GE::GSM;

void GE::GSM::GameStateManager::SetNextScene(std::string next_scene)
{
	auto& scenes =  GE::Assets::AssetManager::GetInstance().GetScenes();
	if (scenes.find(next_scene) == scenes.end())
	{
		GE::Debug::ErrorLogger::GetInstance().LogWarning("Scene " + next_scene + " does not exist. Unable to load");
		return;
	}

	sm.SetNextScene(next_scene);
	m_updated = true;
}

std::string GE::GSM::GameStateManager::GetCurrentScene() const
{
	return sm.GetCurrentScene();
}

void GE::GSM::GameStateManager::SaveScene() const
{
	sm.SaveScene();
}

void GE::GSM::GameStateManager::Restart()
{
	sm.RestartScene();
}

#ifndef IMGUI_DISABLE
void GE::GSM::GameStateManager::LoadSceneFromExplorer(std::string const& filepath)
{
	sm.LoadSceneFromExplorer(filepath);
}
#endif

void GameStateManager::Init()
{
	sm.Init();
	sm.LoadScene();
	sm.InitScene();
}

void GameStateManager::Update()
{
	// We will only reload if the scene is not running
	if (!EditorGUI::ImGuiHelper::IsRunning())
		ExecuteMainThreadQueue();
	try
	{
		if (m_updated)
		{
			if (sm.GetNextScene() == sm.GetCurrentScene())
			{
				sm.RestartScene();
			}
			else
			{

				sm.Update();
			}
			m_updated = false;
		}
		else
		{
			sm.Update();
		}
	}
	catch (GE::Debug::IExceptionBase& e)
	{
		e.LogSource();
	}
}

void GE::GSM::GameStateManager::Exit()
{
	// Example of removing systems
}


void GE::GSM::GameStateManager::SubmitToMainThread(const std::function<void()>& function)
{
	std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);
	m_MainThreadQueue.emplace_back(function);
}

void GE::GSM::GameStateManager::ExecuteMainThreadQueue()
{
	std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

	for (auto& func : m_MainThreadQueue)
		func();

	m_MainThreadQueue.clear();

}