#pragma once
#include <Commands/ICommand.h>
#include <rttr/registration.h>

#include <Component/Transform.h>
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Serialization/Serializer.h>
#include <map>

namespace GE
{
	namespace CMD
	{
    using PRS = std::array<GE::Component::Transform::vec3,3>;

 
    


    class AddObjectCmd : public ICommand {
      PRS m_newTransform;
      GE::ECS::Entity m_entityID;

    public:
      AddObjectCmd(const  PRS&);
      void Execute() override;
      void Undo() override;
      void Redo() override;

      RTTR_ENABLE(ICommand)
    };

    class RemoveObjectCmd : public ICommand {

      struct EntityTemplate
      {
        GE::ECS::Entity m_entityID;
        std::map<ECS::COMPONENT_TYPES, rttr::variant> m_compList;
        std::vector<EntityTemplate> m_childList;
      };

    
      EntityTemplate m_entityData;

    public:
      RemoveObjectCmd( GE::ECS::Entity);
      void Execute() override;
      void Undo() override;
      void Redo() override;
      void RestoreComp(GE::ECS::Entity entityID, std::map<ECS::COMPONENT_TYPES, rttr::variant>& compList);

      RTTR_ENABLE(ICommand)
    };

    class ChangeTransCmd : public ICommand {
      
      PRS m_oldTransform;
      PRS m_newTransform;
      GE::ECS::Entity m_entityID;

    public:
      ChangeTransCmd(const  PRS& , const  PRS&, const  GE::ECS::Entity&);
      void Execute() override;
      void Undo() override;
      void Redo() override;

      RTTR_ENABLE(ICommand)
    };






	}
}
