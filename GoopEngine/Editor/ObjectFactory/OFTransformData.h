#pragma once
#include <pch.h>
#include "ObjectFactory.h"
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Gravity.h>
#include <PlayerController/PlayerControllerSystem.h>
#include <Component/Tween.h>
//#include <sstream>

namespace GE
{
  namespace ObjectFactory
  {
    template<typename CompType>
    CompType DeserializeComponent(std::string const& componentData)
    {
      return CompType();
    }

    template<>
    GE::Component::Transform DeserializeComponent<GE::Component::Transform>(std::string const& componentData)
    {
      rapidjson::Document document{};
      if (document.Parse(componentData.c_str()).HasParseError())
      {
        // throw exception
      }

      const rapidjson::Value& data = document[ECS::componentsToString.at(ECS::COMPONENT_TYPES::TRANSFORM).c_str()];
      //read map, manipulate into trans, return
      GE::Component::Transform trans;
      trans.m_pos << data["m_pos"].GetString();
      trans.m_scale << data["m_scale"].GetString();
      trans.m_rot = data["m_rot"].GetDouble();

      return trans;
    }

    template <>
    GE::Component::BoxCollider DeserializeComponent<GE::Component::BoxCollider>(std::string const& componentData)
    {
      rapidjson::Document document{};
      if (document.Parse(componentData.c_str()).HasParseError())
      {
        // throw exception
      }

      const rapidjson::Value& data = document[ECS::componentsToString.at(ECS::COMPONENT_TYPES::BOXCOLLIDER).c_str()];
      BoxCollider box;
      box.m_width = data["m_width"].GetDouble();
      box.m_height = data["m_height"].GetDouble();
      box.m_min << data["m_min"].GetString();
      box.m_max << data["m_max"].GetString();
      box.m_center << data["m_center"].GetString();
      box.m_mouseCollided = data["m_mouseCollided"].GetBool();

      // may not need to deserialize std::set<BoxCollider*> m_collided

      return box;
    }

    template<>
    GE::Component::Velocity DeserializeComponent<GE::Component::Velocity>(std::string const& componentData)
    {
      rapidjson::Document document{};
      if (document.Parse(componentData.c_str()).HasParseError())
      {
        // throw exception
      }
      const rapidjson::Value& data = document[ECS::componentsToString.at(ECS::COMPONENT_TYPES::VELOCITY).c_str()];
      GE::Component::Velocity vel;

      vel.m_acc.x = data["acc_x"].GetDouble();
      vel.m_acc.y = data["acc_y"].GetDouble();
      vel.m_vel.x = data["vel_x"].GetDouble();
      vel.m_vel.y = data["vel_y"].GetDouble();

      return vel;
    }

    template<>
    GE::Gravity DeserializeComponent<GE::Gravity>(std::string const& componentData)
    {
      //read map, manipulate into trans, return
      rapidjson::Document document{};
      if (document.Parse(componentData.c_str()).HasParseError())
      {
        // throw exception
      }

      const rapidjson::Value& data = document[ECS::componentsToString.at(ECS::COMPONENT_TYPES::GRAVITY).c_str()];
      Gravity grav;
      grav.m_gravity.x = data["grav_x"].GetDouble();
      grav.m_gravity.y = data["grav_y"].GetDouble();

      // may not need to deserialize std::set<BoxCollider*> m_collided

      return grav;
    }
  }
}

