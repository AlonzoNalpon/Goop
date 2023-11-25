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

using namespace GE;

// To ignore during serialization, do
//(
//  metadata("NO_SERIALIZE", true)
//)

RTTR_REGISTRATION
{
  /* ------------------- CLASSES / STRUCTS ------------------- */
  rttr::registration::class_<Math::dVec2>("dVec2")
    .constructor<>()
    .property("x", &Math::dVec2::x)
    .property("y", &Math::dVec2::y)
    .method("ToString", &Math::dVec2::ToString)
    ;
  
  rttr::registration::class_<Math::Vec2>("Vec2")
    .constructor<>()
    .property("x", &Math::Vec2::x)
    .property("y", &Math::Vec2::y)
    .method("ToString", &Math::Vec2::ToString)
    ;

  rttr::registration::class_<Math::dVec3>("dVec3")
    .constructor<>()
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

  rttr::registration::class_<Component::CardHolder::CardHolderEntry>("CardHolderEntry")
    .constructor<>()
    .property("elemEntity", &Component::CardHolder::CardHolderEntry::elemEntity)
    .property("cardEntity", &Component::CardHolder::CardHolderEntry::cardEntity)
    .property("spriteID", &Component::CardHolder::CardHolderEntry::spriteID)
    .property("defaultSpriteID", &Component::CardHolder::CardHolderEntry::defaultSpriteID)
    .property("used", &Component::CardHolder::CardHolderEntry::used)
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
    .constructor<>()
    .property("spriteSubData", &Graphics::SpriteData::info)
    ;

  rttr::registration::class_<GE::MONO::ScriptField>("ScriptField")
    .constructor<>()
    .property("fieldName", &GE::MONO::ScriptField::m_fieldName)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<int>>("System.Int32")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<int>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<int>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<int>::m_scriptField)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<float>>("System.Single")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<float>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<float>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<float>::m_scriptField)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<double>>("System.Double")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<double>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<double>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<double>::m_scriptField)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<unsigned>>("System.UInt32")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<unsigned>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<unsigned>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<unsigned>::m_scriptField)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<std::vector<int>>>("System.Int32[]")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<std::vector<int>>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<std::vector<int>>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<std::vector<int>>::m_scriptField)
    ;
  rttr::registration::class_<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>("System.UInt32[]")
    .constructor<>()
    .property("data", &GE::MONO::ScriptFieldInstance<std::vector<unsigned>>::m_data)
    .property("type", &GE::MONO::ScriptFieldInstance<std::vector<unsigned>>::m_type)
    .property("ScriptField", &GE::MONO::ScriptFieldInstance<std::vector<unsigned>>::m_scriptField)
    ;

  rttr::registration::class_<GE::MONO::ScriptInstance>("ScriptInstance")
    .constructor<>()
    .property("scriptName", &GE::MONO::ScriptInstance::m_scriptName)
    .property("scriptFieldInstList", &GE::MONO::ScriptInstance::m_scriptFieldInstList)
    ;

} // RTTR Registration