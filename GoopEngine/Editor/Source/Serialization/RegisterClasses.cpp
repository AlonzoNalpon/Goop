#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>

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
    .property("worldPos", &Component::Transform::m_worldPos)
    .property("worldScale", &Component::Transform::m_worldScale)
    .property("worldRot", &Component::Transform::m_worldRot)
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
    .constructor<Math::dVec3 const&, double, bool, double>()
    .property("magnitude", &Component::LinearForce::m_magnitude)
    .property("lifetime", &Component::LinearForce::m_lifetime)
    .property("isActive", &Component::LinearForce::m_isActive)
    .property("age", &Component::LinearForce::m_age)
    ;

  rttr::registration::class_<Component::DragForce>("DragForce")
    .constructor<>()
    .constructor<double, bool>()
    .property("magnitude", &Component::DragForce::m_magnitude)
    .property("isActive", &Component::DragForce::m_isActive)
    ;

  rttr::registration::class_<Component::Velocity>("Velocity")
    .constructor<>()
    .constructor<GE::Math::dVec3 const&, GE::Math::dVec3 const&, double, GE::Math::dVec3 const&,
                 Component::DragForce, std::vector<Component::LinearForce>>()
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
    .property("filename", &Component::Sprite::spriteData)  // naming it as filename for now
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .property("name", &Component::SpriteAnim::animID)
    ;

  rttr::registration::class_<Component::Model>("Model")
    .property("mdlID", &Component::Model::mdlID)
    ;

  rttr::registration::class_<Component::ScriptHandler>("ScriptHandler")
    .constructor<std::vector<std::pair<std::string, std::string>> const&, unsigned>()
    .property("scriptMap", &Component::ScriptHandler::m_scriptMap)
    .method("AddScript", &Component::ScriptHandler::AddScript)
    ;

  rttr::registration::class_<Component::Draggable>("Draggable")
    ;

 /* rttr::registration::enumeration<AI::NODE_STATES>("NODE_STATES")
    (
      rttr::value("NEW", AI::NODE_STATES::STATE_NEW),
      rttr::value("RUNNING", AI::NODE_STATES::STATE_RUNNING),
      rttr::value("WAITING", AI::NODE_STATES::STATE_WAITING),
      rttr::value("SUCCEED", AI::NODE_STATES::STATE_SUCCEED),
      rttr::value("FAILED", AI::NODE_STATES::STATE_FAILED),
      rttr::value("COUNT", AI::NODE_STATES::STATE_COUNT)
      );*/
  //rttr::registration::class_<AI::NodeCache>("NodeCache")
  //  .property("nodeID", &Component::NodeCache::m_nodeID)
  //  .property("childIndex", &Component::NodeCache::m_childIndex)
  //  .property("NodeResult", &Component::NodeCache::m_NodeResult)
  //  ;
  rttr::registration::class_<AI::TreeCache>("TreeCache")
    //.property("treeID", &Component::TreeCache::m_treeID)
    //.property("nodeCacheStack", &Component::TreeCache::m_nodeCacheStack)
    //.property("childResult", &Component::TreeCache::m_childResult)
    ;
  rttr::registration::class_<Component::EnemyAI>("EnemyAI")
    .constructor<unsigned, AI::TreeCache const&>()
    .property("entityID", &Component::EnemyAI::m_entityID)
    //.property("enemyTreeCache", &Component::EnemyAI::m_enemyTreeCache)
    ;

  rttr::registration::class_<GE::AI::Node>("Node")
    .property("script", &GE::AI::Node::m_script)
    ;

  /* ------------------- FUNCTIONS ------------------- */

} // RTTR Registration
