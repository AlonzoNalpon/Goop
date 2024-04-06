#pragma once
/*!*********************************************************************
\file   ImGuiCommands.h
\author han.q\@digipen.edu
\date   26-November-2023
\brief
  Contains all the Child Command Classes. Whenever we want to undo/redo 
  a specific action (e.g. delete/add entity, update transform), we will
  create a child command for that action. 

  When the user executes the command, we push it into Command Manager, this
  allows us to undo/redo it later

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef IMGUI_DISABLE
#include <Commands/ICommand.h>
#include <rttr/registration.h>

#include <Component/Transform.h>
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Serialization/Serializer.h>
#include <map>
#include <vector>

namespace GE
{
	namespace CMD
	{
    using PRS = std::array<GE::Component::Transform::vec3,3>;

    class AddObjectCmd : public ICommand {
      PRS m_newTransform;
      GE::ECS::Entity m_entityID;

    public:
      /*!*********************************************************************
      \brief
        This function is called when the user wants to execute a command to add an entity

      \param
        const PRS& newT
        transform of the new Entity
      ************************************************************************/
      AddObjectCmd(const  PRS&);

      /*!*********************************************************************
      \brief
        This function creates a new entity an assign the transform the new entity
      ************************************************************************/
      void Execute() override;

      /*!*********************************************************************
      \brief
        This function undos the entity creattion
      ************************************************************************/
      void Undo() override;

      /*!*********************************************************************
      \brief
        This function redos the entity creattion
      ************************************************************************/
      void Redo() override;

      RTTR_ENABLE(ICommand)
    };

    class RemoveObjectCmd : public ICommand {

      struct EntityTemplate
      {
        GE::ECS::Entity m_entityID;
        std::string m_entityName;
        std::vector<rttr::variant> m_compList;
        std::vector<EntityTemplate> m_childList;
      };

    
      EntityTemplate m_entityData;

    public:
      /*!*********************************************************************
      \brief
        This function is called when the user wants to delete an entity

      \param
        const PRS& newT
        transform of the new Entity
      ************************************************************************/
      RemoveObjectCmd( GE::ECS::Entity);

      /*!*********************************************************************
      \brief
        This function deletes an entity, along with its children too
      ************************************************************************/
      void Execute() override;

      /*!*********************************************************************
      \brief
        This function undos the deletion of entity
      ************************************************************************/
      void Undo() override;

      /*!*********************************************************************
      \brief
        This function Redos the deletion of entity
      ************************************************************************/
      void Redo() override;

      /*!*********************************************************************
      \brief
        This recursive function saves all of the entity's data, including its childrens'
      ************************************************************************/
      EntityTemplate SaveEntityData(GE::ECS::Entity e);

      /*!*********************************************************************
      \brief
        This recursive function restore all of the entity's data, including its childrens'
      ************************************************************************/
      GE::ECS::Entity RestoreEntityData(EntityTemplate& eTemp);
   

      RTTR_ENABLE(ICommand)
    };

    class ChangeTransCmd : public ICommand {
      
      PRS m_oldTransform;
      PRS m_newTransform;
      GE::ECS::Entity m_entityID;

    public:
      /*!*********************************************************************
      \brief
        This function is called when the user wants to change the transform of any entity

      \param
        const PRS& newT
        transform of the new Entity
      ************************************************************************/
      ChangeTransCmd(const  PRS& , const  PRS&, const  GE::ECS::Entity&);

      /*!*********************************************************************
      \brief
        This function updates the new transform of the entity
      ************************************************************************/
      void Execute() override;

      /*!*********************************************************************
      \brief
        This function reverts the entity back to its original transform, before this command was execute
      ************************************************************************/
      void Undo() override;

      /*!*********************************************************************
      \brief
        This function redo's the changing of entity transform
      ************************************************************************/
      void Redo() override;

      RTTR_ENABLE(ICommand)
    };






	}
}
#endif // !IMGUI_DISABLE