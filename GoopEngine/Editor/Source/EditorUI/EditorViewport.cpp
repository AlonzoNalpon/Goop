#include <pch.h>
#include <EditorUI/EditorViewport.h>
#include <Graphics/GraphicsEngine.h>
#include <ImGui/imgui.h>
#include <EditorUI/ImGuiUI.h>

#include <Component/Transform.h>
#include <Component/BoxCollider.h>
void GE::EditorGUI::EditorViewport::UpdateViewport()
{
  auto* ecs = &GE::ECS::EntityComponentSystem::GetInstance();
  auto& gEngine = Graphics::GraphicsEngine::GetInstance();
  GLuint texture = gEngine.GetRenderTexture();

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

  auto& renderer = gEngine.GetRenderer(); // renderer for setting camera
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
          Graphics::Rendering::Camera& camera{ renderer.GetCamera() }; // get reference to camera
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
        Graphics::Rendering::Camera& camera{ renderer.GetCamera() }; // get reference to camera
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
          auto mouseWS{ gEngine.ScreenToWS(MousePosF) };
          double depthVal{ std::numeric_limits<double>::lowest() };
          GE::ECS::Entity selectedID = GE::ECS::INVALID_ID;

          for (GE::ECS::Entity curr : ecs->GetEntities())
          {
            if (!ecs->GetIsActiveEntity(curr))
              continue;

            // get TRANSFORM component
            auto const* colliderPtr = ecs->GetComponent<GE::Component::BoxCollider>(curr);
            auto const* transPtr = ecs->GetComponent<GE::Component::Transform>(curr);
            auto const& trans{ *transPtr };
            if (colliderPtr)
            {
              auto const& coll{ *colliderPtr };
              GE::Math::dVec2 min{ coll.m_min };
              GE::Math::dVec2 max{ coll.m_max };
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
