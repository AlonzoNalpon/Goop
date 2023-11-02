#include <pch.h>
#include <EditorUI/EditorViewport.h>
#include <Graphics/GraphicsEngine.h>
#include <ImGui/imgui.h>
#include <EditorUI/ImGuiUI.h>

#include <Component/Transform.h>
#include <Component/BoxCollider.h>

#include <Component/Sprite.h>
#include <Utilities/GoopUtils.h>
#include <ObjectFactory/ObjectFactory.h>
#include <InputManager/InputManager.h>

void GE::EditorGUI::EditorViewport::UpdateViewport(Graphics::Rendering::FrameBufferInfo & fbInfo)
{
  fbInfo;
  auto* ecs = &GE::ECS::EntityComponentSystem::GetInstance();
  auto& gEngine = Graphics::GraphicsEngine::GetInstance();
  GLuint texture = fbInfo.renderTexture;
  GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };

  // Calculate the UV coordinates based on viewport position and size
  // Get the size of the GLFW window
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 windowSize{ io.DisplaySize.x, io.DisplaySize.y };
  ImVec2 viewportSize = ImGui::GetContentRegionAvail();  // Get the top-left position of the viewport
  ImVec2 viewportPosition = ImGui::GetCursorScreenPos();
  ImVec2 viewportEnd = ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y);
  ImVec2 uv0;
  ImVec2 uv1;

  uv0.x = 1.f + (viewportEnd.x - windowSize.x) / windowSize.x;
  uv1.y = -(viewportPosition.y) / windowSize.y;
  uv0.y = -(1.f + (viewportEnd.y - windowSize.y) / windowSize.y);
  uv1.x = (viewportPosition.x) / windowSize.x;
  // render the image
  ImGui::Image((void*)(intptr_t)texture, viewportSize, uv1, uv0);

  //auto& renderer = gEngine.GetRenderer(); // renderer for setting camera

  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER"))
    {
      if (payload->Data)
      {
        const char* droppedPath = static_cast<const char*>(payload->Data);
        std::string extension = GE::GoopUtils::GetFileExtension(droppedPath);
        auto const mousePosition = GE::Input::InputManager::GetInstance().GetMousePosWorld();

        if (extension == "png")
        {
          GE::ECS::Entity imageEntity = ecs->CreateEntity();
          GE::Component::Transform trans{};
          trans.m_worldPos = { mousePosition.x, mousePosition.y, 0 };
          trans.m_worldScale = { 1, 1, 1 };
          GE::Component::Model mdl{};
          GE::Component::Sprite sprite { gEngine.textureManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath)) };
          
          ecs->AddComponent(imageEntity, trans);
          ecs->AddComponent(imageEntity, mdl);
          ecs->AddComponent(imageEntity, sprite);
          ecs->SetEntityName(imageEntity, GE::GoopUtils::ExtractFilename(droppedPath));
        }

        if (extension == "pfb")
        {
          ECS::Entity entity{ of.SpawnPrefab(GE::GoopUtils::ExtractFilename(droppedPath)) };
          GE::Component::Transform& trans{ *ecs->GetComponent<GE::Component::Transform>(entity) };
          trans.m_worldPos = { mousePosition.x, mousePosition.y, 0 };
        }
      }
    }
    ImGui::EndDragDropTarget();
  }

  if (ImGui::IsMouseHoveringRect(viewportPosition, ImVec2(viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y)))
  {
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
      if (ImGui::IsMouseDown(MOUSE_L_CLICK))
      {
        if (!mouseLHeld)
        {
          mouseLHeld = true;
          auto mouseWS{ gEngine.ScreenToWS(MousePosF, fbInfo.objID) };
          double depthVal{ std::numeric_limits<double>::lowest() };
          GE::ECS::Entity selectedID = GE::ECS::INVALID_ID;

          for (GE::ECS::Entity curr : ecs->GetEntities())
          {
            if (!ecs->GetIsActiveEntity(curr))
              continue;

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
            if (trans.m_pos.z > depthVal)
            {
              depthVal = trans.m_pos.z;
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
  }
}