#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>
#include <Systems/Enemy/EnemySystem.h>

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
  rttr::registration::class_<Math::Vec2>("Vec2")
    .method("ToString", &Math::Vec2::ToString)
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
#ifndef NO_IMGUI
    .property("render", &Component::BoxCollider::m_render)
#endif
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
    .property("filename", &Component::Sprite::m_spriteName)  // naming it as filename for now
    ;

  rttr::registration::class_<Component::SpriteAnim>("SpriteAnim")
    .property("name", &Component::SpriteAnim::m_animID)
    ;

  rttr::registration::class_<Component::Model>("Model")
    .property("mdlID", &Component::Model::m_mdlID)
    ;

  rttr::registration::class_<Component::Draggable>("Draggable")
    ;

  //rttr::registration::class_<AI::NodeCache>("NodeCache")
  //  .property("nodeID", &Component::NodeCache::m_nodeID)
  //  .property("childIndex", &Component::NodeCache::m_childIndex)
  //  .property("NodeResult", &Component::NodeCache::m_NodeResult)
  //  ;
  rttr::registration::class_<Component::EnemyAI>("EnemyAI")
    .constructor<unsigned>()
    .property("treeID", &Component::EnemyAI::m_treeID)
    ;


  rttr::registration::class_<AI::NodeTemplate>("NodeTemplate")
    .property("nodeType", &AI::NodeTemplate::m_nodeType)
    .property("parentNode", &AI::NodeTemplate::m_parentNode)
    .property("childrenNode", &AI::NodeTemplate::m_childrenNode)
    .property("scriptName", &AI::NodeTemplate::m_scriptName)
    .property("pos", &AI::NodeTemplate::m_pos)
    ;
  rttr::registration::class_<AI::TreeTemplate>("TreeTemplate")
    .property("tree", &AI::TreeTemplate::m_tree)
    .property("treeName", &AI::TreeTemplate::m_treeName)
    .property("treeTempID", &AI::TreeTemplate::m_treeTempID)
    ;

  rttr::registration::class_<Component::ScriptHandler>("ScriptHandler")
    .constructor<std::vector<std::string> const&, unsigned>()
    .property("scriptMap", &Component::ScriptHandler::m_scriptMap)
    .method("AddScript", &Component::ScriptHandler::AddScript)
    ;
  // To ignore during serialization
  //(
  //  metadata("NO_SERIALIZE", true)
  //)

   /* ------------------- ENUMERATIONS ------------------- */
  rttr::registration::enumeration<AI::NODE_TYPE>("NODE_TYPE")
    (
      rttr::value("COMPOSITE_NODE", AI::NODE_TYPE::COMPOSITE_NODE),
      rttr::value("LEAF_NODE", AI::NODE_TYPE::LEAF_NODE),
      rttr::value("NODE_TYPE_COUNT", AI::NODE_TYPE::NODE_TYPE_COUNT),
      rttr::value("ROOT_NODE", AI::NODE_TYPE::ROOT_NODE)
      );

  /* ------------------- FUNCTIONS ------------------- */

} // RTTR Registration
