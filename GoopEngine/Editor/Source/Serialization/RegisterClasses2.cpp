/*!*********************************************************************
\file   RegisterClasses2.cpp
\date   3-November-2023
\brief  This file handles the registration of custom classes used in
        our engine to be recognized by RTTR library. Anything that
        needs to be serialized/deserialized needs to be registered.


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>
#include <Systems/Enemy/EnemySystem.h>

using namespace GE;

RTTR_REGISTRATION
{
  /* ------------------- COMPONENTS ------------------- */
  rttr::registration::class_<Graphics::Colorf>("Colorf")
    .property("r", &Graphics::Colorf::r)
    .property("g", &Graphics::Colorf::g)
    .property("b", &Graphics::Colorf::b)
    .property("a", &Graphics::Colorf::a)
    ;
  rttr::registration::class_<Component::Text>("Text")
    .property("text", &Component::Text::m_text)
    .property("clr", &Component::Text::m_clr)
    .property("scale", &Component::Text::m_scale)
    .property("fontID", &Component::Text::m_fontID)
    ;


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

} // RTTR Registration
