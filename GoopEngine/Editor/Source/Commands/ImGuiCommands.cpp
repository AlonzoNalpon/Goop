#include<pch.h>
#include <Commands/ImGuiCommands.h>
#include <Serialization/Serializer.h>

#include <Component/BoxCollider.h>
#include <Component/Camera.h>
#include <Component/Model.h>
#include <Component/ScriptHandler.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/Transform.h>
#include <Component/Tween.h>
#include <Component/Velocity.h>
#include <Component/Draggable.h>
#include <Component/EnemyAI.h>
#include <Component/Camera.h>
#include <Component/Text.h>
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
	for (ECS::COMPONENT_TYPES i{ static_cast<ECS::COMPONENT_TYPES>(0) }; i < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
	{
		GE::Serialization::Serializer* sr = &GE::Serialization::Serializer::GetInstance();
		rttr::variant inst = sr->GetEntityComponent(e, i);

		// skip if component wasn't found
		if (!inst.is_valid()) { continue; }
		m_entityData.m_compList.emplace(i, inst);

	}
}


void RemoveObjectCmd::Execute()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	ecs.DestroyEntity(m_entityData.m_entityID);
}

void RemoveObjectCmd::Undo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	m_entityData.m_entityID = ecs.CreateEntity();
	std::cout << "Create entity\n";
	RestoreComp(m_entityData.m_entityID, m_entityData.m_compList);
}

void RemoveObjectCmd::Redo()
{
	//GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	//ecs.DestroyEntity(m_entityID);
}

void RemoveObjectCmd::RestoreComp(GE::ECS::Entity entityID, std::map<ECS::COMPONENT_TYPES, rttr::variant>& compList)
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::TRANSFORM); it != compList.end())
	{
		std::cout << "RETURN TRANS\n";
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Transform>());
	}
		

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::BOX_COLLIDER); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::BoxCollider>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::VELOCITY); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Velocity>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::SPRITE); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Sprite>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::SPRITE_ANIM); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::SpriteAnim>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::MODEL); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Model>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::TWEEN); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Tween>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::ScriptHandler>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::DRAGGABLE); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::Transform>());

	if (auto it = compList.find(GE::ECS::COMPONENT_TYPES::ENEMY_AI); it != compList.end())
		ecs.AddComponent(entityID, it->second.get_value<GE::Component::EnemyAI>());

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
	GE::Systems::PostRootTransformSystem::Propergate(ecs, m_entityID, trans->m_parentWorldTransform);
}

void ChangeTransCmd::Undo()
{

	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	auto trans = ecs.GetComponent<GE::Component::Transform>(m_entityID);
	trans->m_pos = m_oldTransform[0];
	trans->m_rot = m_oldTransform[1];
	trans->m_scale = m_oldTransform[2];
	GE::Systems::PostRootTransformSystem::Propergate(ecs, m_entityID, trans->m_parentWorldTransform);

}

void ChangeTransCmd::Redo()
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	auto trans = ecs.GetComponent<GE::Component::Transform>(m_entityID);
	trans->m_pos = m_newTransform[0];
	trans->m_rot = m_newTransform[1];
	trans->m_scale = m_newTransform[2];
	GE::Systems::PostRootTransformSystem::Propergate(ecs, m_entityID, trans->m_parentWorldTransform);
}
