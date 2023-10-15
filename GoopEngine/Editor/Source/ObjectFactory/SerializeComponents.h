/*!*********************************************************************
\file   SerializeComponents.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Header for SerializeComponents.tpp.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "ObjectFactory.h"
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>

namespace GE::ObjectFactory
{
  /*!*********************************************************************
  \brief
    Takes the data and assigns it into the entity data.
  \param
    string const& (Identifier of the data located in the json file)
  \return
    COMPONENT_TYPE object
  ************************************************************************/
  template<typename CompType>
  CompType DeserializeComponent(std::string const& componentData);
    
  template<>
  GE::Component::Transform DeserializeComponent<GE::Component::Transform>(std::string const& componentData);

  template <>
  GE::Component::BoxCollider DeserializeComponent<GE::Component::BoxCollider>(std::string const& componentData);

  template<>
  GE::Component::Velocity DeserializeComponent<GE::Component::Velocity>(std::string const& componentData);
    
  template<>
  GE::Component::Sprite DeserializeComponent<GE::Component::Sprite>(std::string const& componentData);
    
  template<>
  GE::Component::SpriteAnim DeserializeComponent<GE::Component::SpriteAnim>(std::string const& componentData);

  template<>
  GE::Component::Model DeserializeComponent<GE::Component::Model>(std::string const& componentData);

  template<typename CompType>
  void SerializeComponent(Serialization::ComponentWrapper const& wrapper) { }

  #include "SerializeComponents.tpp"
}

