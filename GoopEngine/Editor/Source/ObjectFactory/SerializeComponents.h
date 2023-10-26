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
#include <Component/Components.h>
#include <Serialization/ComponentWrapper/ComponentWrapper.h>

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
  template <typename CompType>
  CompType DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::Transform DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::BoxCollider DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::Velocity DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::Sprite DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::SpriteAnim DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::Model DeserializeComponent(std::string const& componentData);
  template<>
  GE::Component::ScriptHandler DeserializeComponent(std::string const& componentData);

}

