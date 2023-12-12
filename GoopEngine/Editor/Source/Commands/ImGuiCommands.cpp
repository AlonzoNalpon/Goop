/*!*********************************************************************
\file   ImGuiCommands.h
\author han.q\@digipen.edu
\date   26 November 2023
\brief
	Contains all the Child Command Classes. Whenever we want to undo/redo
	a specific action (e.g. delete/add entity, update transform), we will
	create a child command for that action.

	When the user executes the command, we push it into Command Manager, this
	allows us to undo/redo it later

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include<pch.h>
#ifndef IMGUI_DISABLE
#include <Commands/ImGuiCommands.h>
#include <Serialization/Serializer.h>
#include <ObjectFactory/ObjectFactory.h>

#include <Component/BoxCollider.h>
#include <Component/Camera.h>
#include <Component/Scripts.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/Transform.h>
#include <Component/Tween.h>
#include <Component/Velocity.h>
#include <Component/Draggable.h>
#include <Component/EnemyAI.h>
#include <Component/Camera.h>
#include <Component/Text.h>
#include <Component/Audio.h>
#include <Component/Button.h>
#include <rttr/registration>





using namespace GE;
using namespace CMD;


// /*!*********************************************************************
//
//    Remove Entity Command
//
//************************************************************************/



RemoveObjectCmd::RemoveObjectCmd(GE::ECS::Entity e)
{
	m_entityData.m_entityID = e;
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

	m_entityData.m_entityName = ecs.GetEntityName(m_entityData.m_entityID);
	m_entityData.m_compList = ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(e);
	std::set<ECS::Entity>& children = ecs.GetChildEntities(e);

	for (ECS::Entity ent : children)
	{
		m_entityData.m_childList.emplace_back(SaveEntityData(ent));
	}
}

RemoveObjectCmd::EntityTemplate RemoveObjectCmd::SaveEntityData(GE::ECS::Entity e)
{
	  GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
		EntityTemplate newEntityTemp;
		newEntityTemp.m_entityID = e;
		newEntityTemp.m_entityName = ecs.GetEntityName(newEntityTemp.m_entityID);
		newEntityTemp.m_compList = ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(e);
		std::set<ECS::Entity>& grandChildren = ecs.GetChildEntities(e);

		for (ECS::Entity ent : grandChildren)
		{
			newEntityTemp.m_childList.emplace_back(SaveEntityData(ent));
		}
		return newEntityTemp;
}


void RemoveObjectCmd::Execute()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	ecs.DestroyEntity(m_entityData.m_entityID);
}

void RemoveObjectCmd::Undo()
{

	//for (rttr::variant inst: m_entityData.m_compList)
	//{
	//	if (inst.get_type().get_wrapped_type() == rttr::type::get<Component::Transform*>())
	//	{
	//		GE::Component::Transform* tr = inst.get_value<GE::Component::Transform*>();
	//		 << tr->m_pos;
	//	}
	//}

	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	m_entityData.m_entityID = ecs.CreateEntity();
	ecs.SetEntityName(m_entityData.m_entityID, m_entityData.m_entityName);
	GE::ObjectFactory::ObjectFactory* oj = &GE::ObjectFactory::ObjectFactory::GetInstance();
	oj->AddComponentsToEntity(m_entityData.m_entityID, m_entityData.m_compList);
	ecs.SetParentEntity(m_entityData.m_entityID);

	for (EntityTemplate& entTemp : m_entityData.m_childList)
	{
		GE::ECS::Entity child = RestoreEntityData(entTemp);
		ecs.SetParentEntity(child, m_entityData.m_entityID);
		ecs.AddChildEntity(m_entityData.m_entityID, child);
	}


}

void RemoveObjectCmd::Redo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	ecs.DestroyEntity(m_entityData.m_entityID);
}

GE::ECS::Entity RemoveObjectCmd::RestoreEntityData(EntityTemplate& eTemp)
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	eTemp.m_entityID = ecs.CreateEntity();
	ecs.SetEntityName(eTemp.m_entityID, eTemp.m_entityName);
	GE::ObjectFactory::ObjectFactory* oj = &GE::ObjectFactory::ObjectFactory::GetInstance();
	oj->AddComponentsToEntity(eTemp.m_entityID, eTemp.m_compList);


	for (EntityTemplate& entTemp : eTemp.m_childList)
	{
		GE::ECS::Entity child = RestoreEntityData(entTemp);
		ecs.SetParentEntity(child, eTemp.m_entityID);
		ecs.AddChildEntity(eTemp.m_entityID, child);
	}

	return eTemp.m_entityID;
}




// /*!*********************************************************************
//
//    Add Entity Command
//
//************************************************************************/



AddObjectCmd::AddObjectCmd(const PRS& newT) :m_newTransform{ newT }, m_entityID{ } {}


void AddObjectCmd::Execute()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	m_entityID = ecs.CreateEntity();
	GE::Component::Transform trans{ m_newTransform[0],m_newTransform[1],m_newTransform[2] };
	ecs.AddComponent(m_entityID, trans);
}

void AddObjectCmd::Undo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	ecs.DestroyEntity(m_entityID);
}

void AddObjectCmd::Redo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	m_entityID = ecs.CreateEntity();
	GE::Component::Transform trans{ m_newTransform[0],m_newTransform[1],m_newTransform[2] };
	ecs.AddComponent(m_entityID, trans);
}





// /*!*********************************************************************
//
//    Change Transform Commands
//
//************************************************************************/


ChangeTransCmd::ChangeTransCmd(const PRS& oldT, const  PRS& newT, const  GE::ECS::Entity& e):m_oldTransform{oldT},m_newTransform{newT},m_entityID{e}{}

void ChangeTransCmd::Execute()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	auto trans = ecs.GetComponent<GE::Component::Transform>(m_entityID);
	GE::Systems::PostRootTransformSystem::Propergate(m_entityID, trans->m_parentWorldTransform);
}

void ChangeTransCmd::Undo()
{

	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	auto trans = ecs.GetComponent<GE::Component::Transform>(m_entityID);
	trans->m_pos = m_oldTransform[0];
	trans->m_rot = m_oldTransform[1];
	trans->m_scale = m_oldTransform[2];
	GE::Systems::PostRootTransformSystem::Propergate(m_entityID, trans->m_parentWorldTransform);

}

void ChangeTransCmd::Redo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	auto trans = ecs.GetComponent<GE::Component::Transform>(m_entityID);
	trans->m_pos = m_newTransform[0];
	trans->m_rot = m_newTransform[1];
	trans->m_scale = m_newTransform[2];
	GE::Systems::PostRootTransformSystem::Propergate(m_entityID, trans->m_parentWorldTransform);
}
#endif