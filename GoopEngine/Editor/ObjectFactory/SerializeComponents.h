#pragma once
#include <pch.h>
#include "ObjectFactory.h"
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Gravity.h>
#include <PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>

namespace GE
{
  namespace ObjectFactory
  {
    template<typename CompType>
    CompType DeserializeComponent(std::string const& componentData);
    
    template<>
    GE::Component::Transform DeserializeComponent<GE::Component::Transform>(std::string const& componentData);

    template <>
    GE::Component::BoxCollider DeserializeComponent<GE::Component::BoxCollider>(std::string const& componentData);

    template<>
    GE::Component::Velocity DeserializeComponent<GE::Component::Velocity>(std::string const& componentData);

    template<>
    GE::Gravity DeserializeComponent<GE::Gravity>(std::string const& componentData);

    #include "SerializeComponents.tpp"
  }
}

