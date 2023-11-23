/*!*********************************************************************
\file   Scene.h
\author loh.j@digipen.edu
\date   03-November-2023
\brief
  Contains the functionality of the main Scene class.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <string>
#include <ECS/EntityComponentSystem.h>
#include <ObjectFactory/ObjectFactory.h>

namespace GE::Scenes
{
  class Scene
  {
  public:
    /*!*********************************************************************
    \brief
      Virtual destroyer to be declared.
    ************************************************************************/
    virtual ~Scene() {};

    /*!*********************************************************************
    \brief
      Loads the scene file and adds all the entities into the relevant maps.

    \param filepath
      filepath in string which should be automatically added by the asset
      manager.
    ************************************************************************/
    virtual void Load(std::string filepath);

    /*!*********************************************************************
    \brief
      Initializes all the entities in the scene file.
    ************************************************************************/
    virtual void Init();

    // No update or render as its part of ecs

    /*!*********************************************************************
    \brief
      Kills all the entities and stops any sound that are currently playing.
    ************************************************************************/
    virtual void Unload();

    /*!*********************************************************************
    \brief
      Unloads all the items in the maps that will not be used anymore.
    ************************************************************************/
    virtual void Free();

    /*!*********************************************************************
    \brief
      Originally a hardcoded scene to test unserialized entities.
    ************************************************************************/
    void TestScene();
  private:
    GE::ECS::EntityComponentSystem* ecs;
    GE::ObjectFactory::ObjectFactory* of;
  };
}
