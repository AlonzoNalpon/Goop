/*!*********************************************************************
\file   AnimEventEditor.cpp
\author a.nalpon\@digipen.edu
\date   8-February-2024
\brief  
This file contains the implementation of the animation events editor.
This editor allows creation of a set of animation events. Each event
is a script name bound to a frame. 
   
 Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved. 
 ************************************************************************/
#include <pch.h>

#ifndef IMGUI_DISABLE
#include <ImGui/imgui.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include <Graphics/GraphicsEngine.h>
#include <EditorUI/AnimEventEditor.h>
#include <Math/GEM.h>
#include <Events/AnimEventManager.h>
namespace GE::EditorGUI
{
  std::string                 AnimEventEditor::currAnimationName{};
  AnimEvents::AnimEventsCont  AnimEventEditor::currAnimationEventsCont{};
  AnimEvents::AnimEventInfo   AnimEventEditor::currAnimationEvents{};
  int                         AnimEventEditor::currAnimFrame{};
  std::string                 AnimEventEditor::currLoadAnimEventID{};

  static constexpr ImVec4 ScriptFieldClr{ 0.4f, 0.4f, 1.f, 1.f };
  static constexpr ImVec4 WarningColor{ 0.8f, 0.f, 0.f, 1.f };
  static constexpr ImVec4 GoodColor{ 0.f, .8f, 0.f, 1.f };
  void AnimEventEditor::CreateContent()
  {
    using namespace ImGui;


    auto& eventMgr = Events::AnimEventManager::GetInstance();
    //auto& animManager = Graphics::GraphicsEngine::GetInstance().GetAnimManager();
    /*float contentSize = GetWindowSize().x;
    BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
    TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
    TableNextColumn();
    EndTable();*/

    //Print currently editing anim name
    BeginChild("Animation Events");
    {
      TextColored({ 1.f, 1.f, 1.f, 1.f }, "Events set name");
      SameLine();

      ImGui::InputText("##nameOfEvents", &currLoadAnimEventID);
      //if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj->animID).c_str()))
    }
    Separator();

    ImGui::Text("Animation Events");
    SameLine();

    auto const& eventsLT = eventMgr.GetAnimEventsTable();
    
    if (BeginCombo("##AnimEventsEdit", currLoadAnimEventID.c_str()))
    {
      for (auto& curr : eventsLT)
      {
        if (curr.first.empty())
          continue;
        if (Selectable(curr.first.c_str(), curr.first == currLoadAnimEventID))
        {
          currLoadAnimEventID = curr.first;
          auto loadedIt = eventsLT.find(currLoadAnimEventID);
          if (loadedIt != eventsLT.end())
          {
            currAnimationEventsCont = loadedIt->second;
          }
        }
      }
      EndCombo();
    }

    SameLine();

    if (Button("LOAD"))
    {
      auto loadedIt = eventsLT.find(currLoadAnimEventID);
      if (loadedIt != eventsLT.end())
      {
        currAnimationEventsCont = loadedIt->second;
      }
    }
    SameLine();

    ImGui::BeginDisabled(currLoadAnimEventID.empty());
    if (Button("SAVE"))
    {
      if (!eventMgr.CreateAnimEvents(currLoadAnimEventID, currAnimationEventsCont, true)) // REMOVE THE LAST BOOL PARAM IF YOU WANT TO HANDLE POPUPS
      {
        // HANDLE POPUPS HERE
      }
    }
    ImGui::EndDisabled();
    
    {
      auto loadedIt = eventsLT.find(currLoadAnimEventID);
      ImGui::BeginDisabled(loadedIt == eventsLT.end());
      if (Button("DELETE"))
      {
        eventMgr.DeleteAnimEvent(currLoadAnimEventID);
      }
      ImGui::EndDisabled();
    }

    // OVERWRITE NOTIFICATION
    if (!currLoadAnimEventID.empty())
    {
      bool uniqueName{};
      try 
      {
        eventMgr.GetAnimEvent(currLoadAnimEventID);
      }
      catch (GE::Debug::Exception<Events::AnimEventManager> c)
      {
        uniqueName = true;
      }
      if (uniqueName)
        TextColored(GoodColor, "This name is available!");
      else
        TextColored(WarningColor, "Saving will overwrite!");

    }
    

    Separator();

    AnimEvents::AnimEventsCont& subject{ currAnimationEventsCont };
    {
      AnimEvents::AnimEventInfo& animEvent{ currAnimationEvents };
      
      ImGui::TextColored(ScriptFieldClr, "Script name");
      SameLine();
      ImGui::InputText("##evtScriptName", &animEvent);

      ImGui::TextColored(ImVec4{ 0.4f, 0.4f, 1.f, 1.f }, "Frame");
      SameLine();
      ImGui::InputInt("##EvtFrameNum", &currAnimFrame);

      AnimEvents::AnimFrameEvents trs;
      // Button to add the event
      if (Button("ADD EVENT"))
      {
        subject[currAnimFrame].push_back(animEvent);
      }
    }


    // The list of events IN ORDER and editable fields
    ImGui::TextColored(ImVec4{ 1.f, .733f, 0.f, 1.f },
      ("ELEMENTS | Size: " + std::to_string(subject.size())).c_str());

    {
      size_t uID{};
      for (auto& currFrame : subject)
      {
        ImGui::TextColored(ImVec4{ 1.f, 1.f, 0.f, 1.f },
          ("Frame: " + std::to_string(currFrame.first)).c_str());

        size_t currEventIdx{};
        for (std::string& currEvt : currFrame.second)
        {
          ImGui::TextColored(ScriptFieldClr, "Script: ");
          ImGui::SameLine();
          ImGui::InputText(("##listedEvtElem" + std::to_string(uID)).c_str(), &currEvt);
          //Delete button
          if (Button(("DELETE##" + std::to_string(uID)).c_str()))
          {
            currFrame.second.erase(currFrame.second.begin() + currEventIdx);
            break; // don't need with displaying for this frame as vector resized
          }
          ++currEventIdx;
          ++uID;
        }
      }
      for (auto removeIt{subject.begin()}; 
        removeIt != subject.end();
        ++removeIt)
      {
        if (removeIt->second.empty())
        {
          subject.erase(removeIt);
          break; // only one can be removed per frame
        }
      }
    }

    EndChild();
    // DO NOT LET THEM SAVE EXISTING ONE
  }
}
#endif
