#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>

using namespace GE;

RTTR_REGISTRATION
{
  /* ------------------- MATH ------------------- */
  rttr::registration::class_<Math::dVec2>("dVec2")
    .constructor<>()
    .constructor<double, double>()
    .constructor<Math::dVec2 const&>()
    .property("x", &Math::dVec2::x)
    .property("y", &Math::dVec2::y)
    .method("ToString", &Math::dVec2::ToString)
    ;

  rttr::registration::class_<Math::dVec3>("dVec3")
    .constructor<>()
    .constructor<double, double, double>()
    .constructor<Math::dVec3 const&>()
    .property("x", &Math::dVec3::x)
    .property("y", &Math::dVec3::y)
    .property("z", &Math::dVec3::z) 
    .method("ToString", &Math::dVec3::ToString)
    ;

  /* ------------------- COMPONENTS ------------------- */
  rttr::registration::class_<Component::Transform>("Transform")
    .constructor<>()
    .constructor<Math::dVec3 const&, Math::dVec3 const&, Math::dVec3 const&>()
    .property("pos", &Component::Transform::m_pos)
    .property("scale", &Component::Transform::m_scale)
    .property("rot", &Component::Transform::m_rot)
    ;

  rttr::registration::class_<Component::BoxCollider>("BoxCollider")
    .constructor<>()
    .constructor<Math::dVec2 const&, double, double>()
    .property("center", &Component::BoxCollider::m_center)
    .property("width", &Component::BoxCollider::m_width)
    .property("height", &Component::BoxCollider::m_height)
    ;

  rttr::registration::class_<Component::LinearForce>("LinearForce")
    .constructor<>()
    .constructor<GE::Math::dVec3 const&, double, bool>()
    .property("magnitude", &Component::LinearForce::m_magnitude)
    .property("lifetime", &Component::LinearForce::m_lifetime)
    .property("isActive", &Component::LinearForce::m_isActive)
    ;

  rttr::registration::class_<Component::DragForce>("DragForce")
    .constructor<>()
    .constructor<GE::Math::dVec3 const&, bool>()
    .property("magnitude", &Component::DragForce::m_magnitude)
    .property("isActive", &Component::DragForce::m_isActive)
    ;

  rttr::registration::class_<Component::Velocity>("Velocity")
    .constructor<>()
    .constructor<GE::Math::dVec3 const&, GE::Math::dVec3 const&, double, GE::Math::dVec3 const&,
                 Component::DragForce const&, std::vector<Component::LinearForce>>()
    .property("vel", &Component::Velocity::m_vel)
    .property("acc", &Component::Velocity::m_acc)
    .property("mass", &Component::Velocity::m_mass)
    .property("gravity", &Component::Velocity::m_gravity)
    .property("dragForce", &Component::Velocity::m_dragForce)
    .property("forces", &Component::Velocity::m_forces)
    .method("AddForce", &Component::Velocity::AddForce)
    ;

  rttr::registration::class_<Component::Tween>("Tween")
    .constructor<double>()
    .property("tweens", &Component::Tween::m_tweens)
    .property("timePerTween", &Component::Tween::m_timePerTween)
    .property("timeTaken", &Component::Tween::m_timeTaken)
    .property("timeElapsed", &Component::Tween::m_timeElapsed)
    .property("originalPos", &Component::Tween::m_originalPos)
    .method("AddTween", &Component::Tween::AddTween)
    ;

  rttr::registration::class_<Graphics::SpriteData>("SpriteData")
    .constructor<>()
    .property("texture", &Graphics::SpriteData::texture)
    ;
  rttr::registration::class_<Component::Sprite>("Sprite")
    .property("texture", &Component::Sprite::spriteData)  // naming it as texture for now
    .method("GetTextureHandle", &Component::Sprite::GetTextureHandle)
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .property("animID", &Component::SpriteAnim::animID)
    ;

  rttr::registration::class_<Component::Model>("Model")
    .property("mdlID", &Component::Model::mdlID)
    ;

  rttr::registration::class_<Component::ScriptHandler>("ScriptHandler")
    .constructor<std::vector<std::pair<std::string, std::string>> const&, unsigned>()
    .property("scriptMap", &Component::ScriptHandler::m_scriptMap)
    .method("AddScript", &Component::ScriptHandler::AddScript)
    ;

} // namespace GE
