/*!*********************************************************************
\file   RegisterClasses.cpp
\date   3-November-2023
\brief  This file handles the registration of custom classes/structs
        used in our engine to be recognized by RTTR library. Anything
        that needs to be serialized/deserialized needs to be registered.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <ScriptEngine/CSharpStructs.h>
#include <Serialization/JsonKeys.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/VariantPrefab.h>
#endif

using namespace GE;

// To ignore during serialization, do
//(
//  metadata("NO_SERIALIZE", true)
//)

RTTR_REGISTRATION
{
  /* ------------------- CLASSES / STRUCTS ------------------- */
  rttr::registration::class_<Math::dVec2>("dVec2")
    .property("x", &Math::dVec2::x)
    .property("y", &Math::dVec2::y)
    .method("ToString", &Math::dVec2::ToString)
    ;
  
  rttr::registration::class_<Math::Vec2>("Vec2")
    .property("x", &Math::Vec2::x)
    .property("y", &Math::Vec2::y)
    .method("ToString", &Math::Vec2::ToString)
    ;

  rttr::registration::class_<Math::dVec3>("dVec3")
    .property("x", &Math::dVec3::x)
    .property("y", &Math::dVec3::y)
    .property("z", &Math::dVec3::z) 
    .method("ToString", &Math::dVec3::ToString)
    ;

  rttr::registration::class_<glm::vec2>("glm_vec2")
    .constructor<>()
    .property("x", &glm::vec2::x)
    .property("y", &glm::vec2::y)
    ;

  rttr::registration::class_<Component::LinearForce>("LinearForce")
    .constructor<>()
    .property("magnitude", &Component::LinearForce::m_magnitude)
    .property("lifetime", &Component::LinearForce::m_lifetime)
    .property("isActive", &Component::LinearForce::m_isActive)
    .property("age", &Component::LinearForce::m_age)
    ;

  rttr::registration::class_<Component::DragForce>("DragForce")
    .constructor<>()
    .property("magnitude", &Component::DragForce::m_magnitude)
    .property("isActive", &Component::DragForce::m_isActive)
    ;

  rttr::registration::class_<Graphics::Colorf>("Colorf")
    .property("r", &Graphics::Colorf::r)
    .property("g", &Graphics::Colorf::g)
    .property("b", &Graphics::Colorf::b)
    .property("a", &Graphics::Colorf::a)
    ;

  rttr::registration::class_<Component::Tween::Action>("Action")
    .constructor<>()
    .property("trans", &Component::Tween::Action::m_trans)
    .property("scale", &Component::Tween::Action::m_scale)
    .property("rot", &Component::Tween::Action::m_rot)
    .property("duration", &Component::Tween::Action::m_duration)
    ;

  rttr::registration::class_<Component::Audio::Sound>("Sound")
    .property("sound", &Component::Audio::Sound::m_sound)
    .property("loop", &Component::Audio::Sound::m_loop)
    .property("playOnStart", &Component::Audio::Sound::m_playOnStart)
    .property("channel", &Component::Audio::Sound::m_channel)
    .property("volume", &Component::Audio::Sound::m_volume)
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
  rttr::registration::class_<Deck>("Deck")
    .property("Cards In Deck", &Deck::m_cards)
    .property("Draw Order", &Deck::m_drawOrderDisplay)
    ;


  rttr::registration::class_<DeckManager>("DeckManager")
    .property("Cards In Discard", &DeckManager::m_discardDisplay)
    .property("Cards In Hand", &DeckManager::m_hand)
    .property("Cards In Queue", &DeckManager::m_queue)
    ;

  rttr::registration::class_<HealthBar>("HealthBar")
    .property("m_health", &HealthBar::m_health)
    .property("m_maxHealth", &HealthBar::m_maxHealth)
    .property("m_healthBarUI", &HealthBar::m_healthBarUI)
    ;


  rttr::registration::class_<Component::CardHolder::CardHolderEntry>("CardHolderEntry")
    .property("elemEntity", &Component::CardHolder::CardHolderEntry::elemEntity)
    .property("cardEntity", &Component::CardHolder::CardHolderEntry::cardEntity)
    .property("spriteID", &Component::CardHolder::CardHolderEntry::spriteID)
    .property("defaultSpriteID", &Component::CardHolder::CardHolderEntry::defaultSpriteID)
    .property("used", &Component::CardHolder::CardHolderEntry::used)
    ;

  rttr::registration::class_<Component::Audio::Sound>("Sound")
    .property("sound", &Component::Audio::Sound::m_sound)
    .property("loop", &Component::Audio::Sound::m_loop)
    .property("playOnStart", &Component::Audio::Sound::m_playOnStart)
    .property("channel", &Component::Audio::Sound::m_channel)
    .property("volume", &Component::Audio::Sound::m_volume)
    ;

  rttr::registration::class_<Graphics::SpriteSubData>("SpriteSubData")
    /*.constructor<Graphics::gVec2, Graphics::gVec2, GLuint, GLuint>()
    (
      rttr::parameter_names("texCoords", "texDims", "width", "height")
    )*/
    .property("texCoords", &Graphics::SpriteSubData::texCoords)
    .property("texDims", &Graphics::SpriteSubData::texDims)
    .property("width", &Graphics::SpriteSubData::width)
    .property("height", &Graphics::SpriteSubData::height)
    ;
  rttr::registration::class_<Graphics::SpriteData>("SpriteData")
    .property("spriteSubData", &Graphics::SpriteData::info)
    ;

  rttr::registration::class_<MONO::ScriptField>("ScriptField")
    .property("fieldName", &MONO::ScriptField::m_fieldName)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<int>>("System.Int32")
    .property("data", &MONO::ScriptFieldInstance<int>::m_data)
    .property("type", &MONO::ScriptFieldInstance<int>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<int>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<float>>("System.Single")
    .property("data", &MONO::ScriptFieldInstance<float>::m_data)
    .property("type", &MONO::ScriptFieldInstance<float>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<float>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<double>>("System.Double")
    .property("data", &MONO::ScriptFieldInstance<double>::m_data)
    .property("type", &MONO::ScriptFieldInstance<double>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<double>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<unsigned>>("System.UInt32")
    .property("data", &MONO::ScriptFieldInstance<unsigned>::m_data)
    .property("type", &MONO::ScriptFieldInstance<unsigned>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<unsigned>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<unsigned>>("System.UInt32")
    .property("data", &MONO::ScriptFieldInstance<unsigned>::m_data)
    .property("type", &MONO::ScriptFieldInstance<unsigned>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<unsigned>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<std::vector<int>>>("System.Int32[]")
    .property("data", &MONO::ScriptFieldInstance<std::vector<int>>::m_data)
    .property("type", &MONO::ScriptFieldInstance<std::vector<int>>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<std::vector<int>>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<std::vector<unsigned>>>("System.UInt32[]")
    .property("data", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_data)
    .property("type", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<Math::dVec3>>("GoopScripts.Mono.Vec3<System.Double>")
    .property("data", &MONO::ScriptFieldInstance<Math::dVec3>::m_data)
    .property("type", &MONO::ScriptFieldInstance<Math::dVec3>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<Math::dVec3>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<std::vector<unsigned>>>("GoopScripts.Cards.CardBase.CardID[]")
    .property("data", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_data)
    .property("type", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<std::vector<unsigned>>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<DeckManager>>("GoopScripts.Gameplay.DeckManager")
    .property("data", &MONO::ScriptFieldInstance<DeckManager>::m_data)
    .property("type", &MONO::ScriptFieldInstance<DeckManager>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<DeckManager>::m_scriptField)
    ;
  rttr::registration::class_<MONO::ScriptFieldInstance<CharacterType>>("GoopScripts.Gameplay.CharacterType")
    .property("data", &MONO::ScriptFieldInstance<CharacterType>::m_data)
    .property("type", &MONO::ScriptFieldInstance<CharacterType>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<CharacterType>::m_scriptField)
    ;

  rttr::registration::class_<MONO::ScriptFieldInstance<HealthBar>>("GoopScripts.Gameplay.HealthBar")
    .property("data", &MONO::ScriptFieldInstance<HealthBar>::m_data)
    .property("type", &MONO::ScriptFieldInstance<HealthBar>::m_type)
    .property("scriptField", &MONO::ScriptFieldInstance<HealthBar>::m_scriptField)
    ;


  rttr::registration::class_<MONO::ScriptInstance>("ScriptInstance")
    .property("scriptName", &MONO::ScriptInstance::m_scriptName)
    .property("entityID", &MONO::ScriptInstance::m_entityID)
    .property("scriptFieldInstList", &MONO::ScriptInstance::m_scriptFieldInstList)
    ;

  rttr::registration::class_<std::pair<Graphics::gObjID, std::string>>("SizeTString")
    .property("first", &std::pair<Graphics::gObjID, std::string>::first)
    .property("second", &std::pair<Graphics::gObjID, std::string>::second)
    ;

#ifndef NO_IMGUI
  rttr::registration::class_<std::pair<std::string, unsigned>>("StringUnsignedPair")
    .property("first", &std::pair<std::string, unsigned>::first)
    .property("second", &std::pair<std::string, unsigned>::second)
    ;

  rttr::registration::class_<Prefabs::VariantPrefab::EntityMappings>("EntityMappings")
    .property("name", &Prefabs::VariantPrefab::EntityMappings::m_prefab)
    .property("version", &Prefabs::VariantPrefab::EntityMappings::m_version)
    .property("objToEntity", &Prefabs::VariantPrefab::EntityMappings::m_objToEntity)
    .property("registered", &Prefabs::VariantPrefab::EntityMappings::m_registered)
    ;

  rttr::registration::class_<Prefabs::VariantPrefab::RemovedComponent>("RemovedComponent")
    .property("id", &Prefabs::VariantPrefab::RemovedComponent::m_id)
    .property("type", &Prefabs::VariantPrefab::RemovedComponent::m_type)
    .property("version", &Prefabs::VariantPrefab::RemovedComponent::m_version)
    ;

  rttr::registration::class_<std::pair<Prefabs::PrefabSubData::SubDataId, Prefabs::PrefabVersion>>("UnsignedUnsignedPair")
    .property("first", &std::pair<Prefabs::PrefabSubData::SubDataId, Prefabs::PrefabVersion>::first)
    .property("second", &std::pair<Prefabs::PrefabSubData::SubDataId, Prefabs::PrefabVersion>::second)
    ;

  rttr::registration::class_<rttr::type>("RttrType");
#endif

} // RTTR Registration
