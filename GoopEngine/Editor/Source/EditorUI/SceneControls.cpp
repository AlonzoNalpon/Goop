/*!**********************************************************************
\file   SceneControls.h
\author w.chinkitbryam\@digipen.edu
\date   25 October 2023
\brief
  Wrapper class to create an EditorGUI for scene controls

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "SceneControls.h"
#include "ImGuiUI.h"
#include <Events/EventManager.h>
#include "PrefabEditor.h"

using namespace ImGui;
using namespace GE::EditorGUI;

void GE::EditorGUI::SceneControls::CreateContent()
{
  char const play[] = "Play", stop[] = "Stop", pause[] = "Pause", step[] = "Step";
  BeginDisabled(PrefabEditor::IsEditingPrefab());
  if (ImGui::BeginMenuBar())
  {
    if (ImGuiHelper::IsRunning())
    {
      if (Button(stop))
      {
        GE::fMOD::FmodSystem::GetInstance().StopAllSound();
        Events::EventManager::GetInstance().Dispatch(Events::StopSceneEvent());
        ImGuiHelper::Restart();
        GE::FPS::FrameRateController::GetInstance().SetTimeScale(1.f);
        auto& fmod = GE::fMOD::FmodSystem::GetInstance();
        fmod.SetChannelPause(GE::fMOD::FmodSystem::BGM, true);
        fmod.SetChannelPause(GE::fMOD::FmodSystem::SFX, true);
        fmod.SetChannelPause(GE::fMOD::FmodSystem::VOICE, true);
      }
      if (Button(pause))
      {
        ImGuiHelper::Pause();
      }
      BeginDisabled();
      Button(step);
      EndDisabled();
    }
    else if (ImGuiHelper::Paused())
    {
      if (Button(play))
      {
        ImGuiHelper::Play();
      }
      BeginDisabled();
      Button(pause);
      EndDisabled();
      if (Button(step))
      {
        ImGuiHelper::StepSimulation(true);
      }      
    }
    else
    {
      if (Button(play))
      {
        Events::EventManager::GetInstance().Dispatch(Events::StartSceneEvent());
        ImGuiHelper::Play();
      }
      BeginDisabled();
      Button(pause);
      Button(step);
      EndDisabled();
    }

    EndMenuBar();
  }
  EndDisabled();
}
#endif