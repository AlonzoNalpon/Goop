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


  // To ignore during serialization
  //(
  //  metadata("NO_SERIALIZE", true)
  //)

   /* ------------------- ENUMERATIONS ------------------- */
  rttr::registration::enumeration<ECS::COMPONENT_TYPES>("COMPONENT_TYPES")
    (
      rttr::value("TRANSFORM", ECS::COMPONENT_TYPES::TRANSFORM),
      rttr::value("BOX_COLLIDER", ECS::COMPONENT_TYPES::BOX_COLLIDER),
      rttr::value("VELOCITY", ECS::COMPONENT_TYPES::VELOCITY),
      rttr::value("SPRITE", ECS::COMPONENT_TYPES::SPRITE),
      rttr::value("SPRITE_ANIM", ECS::COMPONENT_TYPES::SPRITE_ANIM),
      rttr::value("MODEL", ECS::COMPONENT_TYPES::MODEL),
      rttr::value("TWEEN", ECS::COMPONENT_TYPES::TWEEN),
      rttr::value("SCRIPT_HANDLER", ECS::COMPONENT_TYPES::SCRIPT_HANDLER),
      rttr::value("DRAGGABLE", ECS::COMPONENT_TYPES::DRAGGABLE),
      rttr::value("ENEMY_AI", ECS::COMPONENT_TYPES::ENEMY_AI),
      rttr::value("COMPONENTS_TOTAL", ECS::COMPONENT_TYPES::COMPONENTS_TOTAL)
      );

  rttr::registration::enumeration<ECS::SYSTEM_TYPES>("SYSTEM_TYPES")
    (
      rttr::value("TRANSFORM", ECS::SYSTEM_TYPES::COLLISION),
      rttr::value("PHYSICS", ECS::SYSTEM_TYPES::PHYSICS),
      rttr::value("RENDERING", ECS::SYSTEM_TYPES::RENDERING),
      rttr::value("SPRITE_ANIM", ECS::SYSTEM_TYPES::SPRITE_ANIM),
      rttr::value("DRAGGABLE_OBJECT", ECS::SYSTEM_TYPES::DRAGGABLE_OBJECT),
      rttr::value("PLAYER_CONTROLLER", ECS::SYSTEM_TYPES::PLAYER_CONTROLLER),
      rttr::value("POST_ROOT_TRANSFORM", ECS::SYSTEM_TYPES::POST_ROOT_TRANSFORM),
      rttr::value("PRE_ROOT_TRANSFORM", ECS::SYSTEM_TYPES::PRE_ROOT_TRANSFORM),
      rttr::value("ENEMY_SYSTEM", ECS::SYSTEM_TYPES::ENEMY_SYSTEM),
      rttr::value("TOTAL_SYSTEMS", ECS::SYSTEM_TYPES::TOTAL_SYSTEMS)
      );

  rttr::registration::enumeration<AI::NODE_TYPE>("NODE_TYPE")
    (
      rttr::value("COMPOSITE_NODE", AI::NODE_TYPE::COMPOSITE_NODE),
      rttr::value("LEAF_NODE", AI::NODE_TYPE::LEAF_NODE),
      rttr::value("NODE_TYPE_COUNT", AI::NODE_TYPE::NODE_TYPE_COUNT),
      rttr::value("ROOT_NODE", AI::NODE_TYPE::ROOT_NODE)
      );

  /* ------------------- FUNCTIONS ------------------- */

} // RTTR Registration
