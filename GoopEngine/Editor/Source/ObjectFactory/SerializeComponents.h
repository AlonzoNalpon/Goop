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
#include <Component/BoxCollider.h>
#include <Component/Tween.h>
#include <Component/Model.h>
#include <Component/ScriptHandler.h>

namespace GE::ObjectFactory
{
  /*!*********************************************************************
  \brief
    Takes the raw data of a component (in string) and deserializes it
    into values for the component
  \param componentData
    The data of the component in string
  \return
    The component initialized with the values from the file
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

  template<>
  GE::Component::ScriptHandler DeserializeComponent<GE::Component::ScriptHandler>(std::string const& componentData);
}

