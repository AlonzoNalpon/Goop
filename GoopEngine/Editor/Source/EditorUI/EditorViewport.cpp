/*!*********************************************************************
\file   EditorViewport.cpp
\author a.nalpon\@digipen.edu
\co-authors 
        loh.j\@digipen.edu
\date   3-November-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef NO_IMGUI
#include <EditorUI/EditorViewport.h>
#include <Graphics/GraphicsEngine.h>
#include <ImGui/imgui.h>
#include <EditorUI/ImGuiUI.h>
#include <Events/InputEvents.h>
#include <Commands/CommandManager.h>


#include <Component/Transform.h>
#include <Component/BoxCollider.h>

#include <Component/Sprite.h>
#include <ObjectFactory/ObjectFactory.h>
#include <InputManager/InputManager.h>
#include <filesystem>
#include <GameStateManager/GameStateManager.h>
#include <ImGuizmo_1_83/ImGuizmo.h>


namespace GE::EditorGUI

{

  namespace // static variables
  // Why the need for these?
  //  These values change to bad ones when I get them from ImGui after rendering (bottom func)
  //  So now rendering will store the right ones here first
  {
    ImVec2 windowSize;
    ImVec2 viewportSize;  // Get the top-left position of the viewport
    ImVec2 viewportPosition;
    ImVec2 viewportEnd;
    ImVec2 uv0;
    ImVec2 uv1;
  }

  bool EditorViewport::focused{};
  bool EditorViewport::m_deleteKeyTriggered{ false };
  bool const& EditorViewport::isFocused{ focused };


  void EditorViewport::UpdateViewport(Graphics::Rendering::FrameBufferInfo& fbInfo)
  {
    auto* ecs = &GE::ECS::EntityComponentSystem::GetInstance();
    auto& gEngine = Graphics::GraphicsEngine::GetInstance();
    GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };

    // Calculate the UV coordinates based on viewport position and size
    // Get the size of the GLFW window
    ImGuiIO& io = ImGui::GetIO();

    uv0.x = 1.f + (viewportEnd.x - windowSize.x) / windowSize.x;
    uv1.y = -(viewportPosition.y) / windowSize.y;
    uv0.y = -(1.f + (viewportEnd.y - windowSize.y) / windowSize.y);
    uv1.x = (viewportPosition.x) / windowSize.x;
    // render the image
#if ORIGINAL
    GLuint texture = fbInfo.renderTexture;
    ImGui::Image((void*)(intptr_t)texture, viewportSize, uv1, uv0);
#endif
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_IMAGE"))
      {
        const char* droppedPath = static_cast<const char*>(payload->Data);
        std::string extension = GE::GoopUtils::GetFileExtension(droppedPath);
        auto const mousePosition = GE::Input::InputManager::GetInstance().GetMousePosWorld();

        if (extension == "png")
        {
          GLuint textureID = gEngine.textureManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath));
          GE::ECS::Entity imageEntity = ecs->CreateEntity();
          GE::Component::Transform trans{};
          trans.m_worldPos = { mousePosition.x, mousePosition.y, 0 };
          trans.m_worldScale = { 1, 1, 1 };
          GE::Component::Sprite sprite{ textureID };
          GE::Component::BoxCollider boxCollider{ {mousePosition}, static_cast<double>(gEngine.textureManager.GetTexture(textureID).width), static_cast<double>(gEngine.textureManager.GetTexture(textureID).height) };
#ifndef NO_IMGUI
          boxCollider.m_render = true;
#endif
          ecs->AddComponent(imageEntity, trans);
          ecs->AddComponent(imageEntity, sprite);
          ecs->AddComponent(imageEntity, boxCollider);
          ecs->SetEntityName(imageEntity, GE::GoopUtils::ExtractFilename(droppedPath));
        }
      }
      if (const ImGuiPayload* payload2 = ImGui::AcceptDragDropPayload("ASSET_BROWSER_PREFAB"))
      {
        IM_ASSERT(payload2->DataSize == sizeof(std::filesystem::path));
        std::filesystem::path const filepath{ *reinterpret_cast<std::filesystem::path*>(payload2->Data) };
        std::string const extension = filepath.extension().string();
        if (extension == ".pfb")
        {
          auto const mousePosition = GE::Input::InputManager::GetInstance().GetMousePosWorld();
          ECS::Entity entity{ of.SpawnPrefab(filepath.stem().string()) };
          GE::Component::Transform& trans{ *ecs->GetComponent<GE::Component::Transform>(entity) };
          trans.m_worldPos = { mousePosition.x, mousePosition.y, 0 };
        }
        // retrieve payload and cast back to base type]
      }
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_SCENE"))
      {
        const char* droppedPath = static_cast<const char*>(payload->Data);
        GE::GSM::GameStateManager::GetInstance().SetNextScene(GE::GoopUtils::ExtractFilename(droppedPath));
      }

      ImGui::EndDragDropTarget();
    }

    //if (ImGui::IsMouseHoveringRect(viewportPosition, ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y))) // OLD CODE
    if (ImGui::IsWindowFocused() && ImGui::IsMouseHoveringRect(viewportPosition, ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y)))
    {
      focused = true;
      ImVec2 mousePosition = ImGui::GetMousePos();
      mousePosition.y = windowSize.y - mousePosition.y;
      static Graphics::gVec2 prevPos; // previous mouse position

      Graphics::gVec2 MousePosF{ mousePosition.x, mousePosition.y }; // current position of mouse in float
      // If the middle mouse button is down (for moving camera)
      {
        constexpr int MIDDLE_MOUSE{ 2 };        // constant for middle mouse key
        static bool middleMouseHeld{};          // flag for middle mouse held down
        if (ImGui::IsMouseDown(MIDDLE_MOUSE))
        {

          if (middleMouseHeld) // check if it's not the first frame button is held
          {
            Graphics::gVec2 displacement{ prevPos - MousePosF };  // displacement of camera
            Graphics::Rendering::Camera& camera{ fbInfo.camera }; // get reference to camera
            camera.DisplaceCam({ displacement.x, displacement.y, 0.f });
            prevPos = { MousePosF };
          }
          else // else this is the first frame we are holding it down
          {
            prevPos = { MousePosF };
            middleMouseHeld = true;
          }
        }
        else {
          middleMouseHeld = false;
        }
      }
      // If the mousewheel is scrolled (for zooming camera)
      {
        float scrollValue = io.MouseWheel;
        constexpr float MULTIPLIER = 10.f;
        if (scrollValue)
        {
          Graphics::Rendering::Camera& camera{ fbInfo.camera }; // get reference to camera
          camera.ZoomCamera(scrollValue * MULTIPLIER);
        }
      }

      // If the mouse clicked was detected
      {
        constexpr int MOUSE_L_CLICK{ 0 };        // constant for mouse left click
        static bool mouseLHeld{};
        if (ImGui::IsMouseDown(MOUSE_L_CLICK) && !ImGuizmo::IsUsing()) // BUT NOT IF IMGUIZMO IS BEING USED
        {
          if (!mouseLHeld)
          {
            mouseLHeld = true;
            auto mouseWS{ gEngine.ScreenToWS(MousePosF, fbInfo.objID) };
            double depthVal{ std::numeric_limits<double>::lowest() };
            GE::ECS::Entity selectedID = GE::ECS::INVALID_ID;

            for (GE::ECS::Entity curr : ecs->GetEntities())
            {
              // Check if this entity is updatable
              {
                GE::ECS::Entity parent{ curr };
                bool active{ true };
                while (parent != GE::ECS::INVALID_ID)
                {
                  if (!ecs->GetIsActiveEntity(parent))
                  {
                    active = false;
                    break;
                  }

                  parent = ecs->GetParentEntity(parent);
                }
                if (!active)
                  continue;
              }

              auto const* transPtr = ecs->GetComponent<GE::Component::Transform>(curr);
              auto const& trans{ *transPtr };
              // CASE 1: CHECKING WITH COLLIDER
              if (ecs->HasComponent<GE::Component::BoxCollider>(curr))
              {
                auto const* colliderPtr = ecs->GetComponent<GE::Component::BoxCollider>(curr);
                auto const& coll{ *colliderPtr };
                GE::Math::dVec2 min{ coll.m_min };
                GE::Math::dVec2 max{ coll.m_max };
                // AABB check with the mesh based on its COLLIDER
                if (min.x > mouseWS.x ||
                  max.x < mouseWS.x ||
                  min.y > mouseWS.y ||
                  max.y < mouseWS.y)
                  continue;
              }
              // CASE 2: CHECKING WITH TEXTURE
              else if (ecs->HasComponent<GE::Component::Sprite>(curr))
              {
                GE::Component::Sprite* sprite = ecs->GetComponent<GE::Component::Sprite>(curr);
                GE::Math::dVec2 min{ trans.m_pos.x - sprite->m_spriteData.info.width * trans.m_scale.x * 0.5,
                  trans.m_pos.y - sprite->m_spriteData.info.height * trans.m_scale.y * 0.5 };
                GE::Math::dVec2 max{ trans.m_pos.x + sprite->m_spriteData.info.width * trans.m_scale.x * 0.5,
                  trans.m_pos.y + sprite->m_spriteData.info.height * trans.m_scale.y * 0.5 };

                // AABB check with the mesh based on its transform (ASSUMES A SQUARE)
                if (min.x > mouseWS.x ||
                  max.x < mouseWS.x ||
                  min.y > mouseWS.y ||
                  max.y < mouseWS.y)
                  continue;
              }
              else
              {
                GE::Math::dVec2 min{ trans.m_pos.x - trans.m_scale.x * 0.5, trans.m_pos.y - trans.m_scale.y * 0.5 };
                GE::Math::dVec2 max{ trans.m_pos.x + trans.m_scale.x * 0.5, trans.m_pos.y + trans.m_scale.y * 0.5 };

                // AABB check with the mesh based on its transform (ASSUMES A SQUARE)
                if (min.x > mouseWS.x ||
                  max.x < mouseWS.x ||
                  min.y > mouseWS.y ||
                  max.y < mouseWS.y)
                  continue;
              }

              // Depth check (only take frontmost object
              if (trans.m_worldPos.z > depthVal)
              {
                depthVal = trans.m_worldPos.z;
                selectedID = curr;
              }
            }
            // Set selected entity (invalid ID means none selected)
            ImGuiHelper::SetSelectedEntity(selectedID);
          }
        }
        else
          mouseLHeld = false;
      }

      // If the delete key is pressed, delete the selected en
      if (m_deleteKeyTriggered && !ImGuizmo::IsUsing() && ImGuiHelper::GetSelectedEntity() != GE::ECS::INVALID_ID)
      {
        GE::CMD::RemoveObjectCmd newRemCmd = GE::CMD::RemoveObjectCmd(ImGuiHelper::GetSelectedEntity());
        GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
        cmdMan.AddCommand(newRemCmd);
        m_deleteKeyTriggered = false;
        GE::ECS::Entity Inv = GE::ECS::INVALID_ID;
        ImGuiHelper::SetSelectedEntity(Inv);
      }
    }
    else
      focused = false;
  }

  void EditorViewport::RenderViewport(Graphics::Rendering::FrameBufferInfo& fbInfo)
  {
    // Calculate the UV coordinates based on viewport position and size
    // Get the size of the GLFW window
    ImGuiIO& io = ImGui::GetIO();
    windowSize = { io.DisplaySize.x, io.DisplaySize.y };
    viewportSize = ImGui::GetContentRegionAvail();  
    viewportPosition = ImGui::GetCursorScreenPos(); // Get the top-left position of the viewport
    viewportEnd = ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y);

    uv0.x = 1.f + (viewportEnd.x - windowSize.x) / windowSize.x;
    uv1.y = -(viewportPosition.y) / windowSize.y;
    uv0.y = -(1.f + (viewportEnd.y - windowSize.y) / windowSize.y);
    uv1.x = (viewportPosition.x) / windowSize.x;
    // render the image
    //auto& renderer = gEngine.GetRenderer(); // renderer for setting camera
    GLuint texture = fbInfo.renderTexture;
    ImGui::Image((void*)(intptr_t)texture, viewportSize, uv1, uv0);
  }

  void EditorViewport::HandleEvent(Events::Event* event)
  {
    if (event->GetCategory() == Events::EVENT_TYPE::KEY_TRIGGERED)
    {

      if (static_cast<Events::KeyTriggeredEvent*>(event)->GetKey() == GPK_DELETE)
      {
        m_deleteKeyTriggered = true;
      }
    }
  }

}


#endif