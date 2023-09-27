#pragma once
#include <pch.h>
#include "ObjectFactory.h"
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Gravity.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
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
    GE::Component::Gravity DeserializeComponent<GE::Component::Gravity>(std::string const& componentData);
    
    template<>
    GE::Component::Sprite DeserializeComponent<GE::Component::Sprite>(std::string const& componentData);
    
    template<>
    GE::Component::SpriteAnim DeserializeComponent<GE::Component::SpriteAnim>(std::string const& componentData);

    template<>
    GE::Component::Model DeserializeComponent<GE::Component::Model>(std::string const& componentData);

    template<typename CompType>
    void SerializeComponent(Serialization::ComponentWrapper const& wrapper)
    {

    }

    #include "SerializeComponents.tpp"
  }
}

