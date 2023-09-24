#include <Systems/Rendering/RenderingSystem.h>
#include <FrameRateController/FrameRateController.h>
#include <Graphics/GraphicsEngine.h>
namespace GE::Systems
{
  void RenderSystem::Update()
  {
    Graphics::GraphicsEngine& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    double dt{ FPS::FrameRateController::GetInstance().GetDeltaTime() };
    f32 dtF{ static_cast<f32>(dt) };
    for (GE::ECS::Entity entity : m_entities)
    {
      Component::Model*       model{ m_ecs->GetComponent<Component::Model>(entity) };
      Component::Sprite*      sprite{ m_ecs->GetComponent<Component::Sprite>(entity) };
      Component::SpriteAnim*  animData{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      Component::Transform* transform{ m_ecs->GetComponent<Component::Transform>(entity) };
      // Updating the sprite anim data (if any)
      if (animData)
      {
        // get the sprite animation
        Graphics::SpriteAnimation const& spriteAnim
          { gEngine.animManager.GetAnim(animData->animID) };

        // Updating sprite animation data
        {
          // Update the timer and see if frame should be changed
          animData->currTime += dt;
          if (animData->currTime >= spriteAnim.speed)
          {
            animData->currTime -= spriteAnim.speed; // reset speed for next frame
            // move to next frame, or wrap back to start
            u32 const newFrame{ (animData->currFrame + 1) };
            animData->currFrame = (newFrame >= spriteAnim.frames.size() ?
              0 : newFrame);
          }
        }

        // setting the new sprite based on data
        sprite->spriteData.info = spriteAnim.frames[animData->currFrame];
      }
      
      // Rendering
      
      Graphics::Rendering::Renderer& renderer{ gEngine.GetRenderer() };
      // Render the object
      GLfloat scale{ 100.f }; // we are temporarily increasing the scale to this value for now
      renderer.RenderObject(model->mdlID, sprite->spriteData,
        Graphics::Rendering::Transform{
          //{ transform->m_scale.x, transform->m_scale.y, 1.f},
          { scale,scale, 1.f},
            static_cast<GLfloat>(transform->m_rot),
            { static_cast<GLfloat>(transform->m_pos.x), static_cast<GLfloat>(transform->m_pos.y), 0.f }
        });
    }
  }

  void RenderSystem::OnDestroyed()
  {
  }

}