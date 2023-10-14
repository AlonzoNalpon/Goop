/*!******************************************************************
\file   SceneHeirachy.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines the behaviour and logic for the scene heirachy dock
********************************************************************/
#include <pch.h>
#include "SceneHierachy.h"
#include <ImGui/imgui.h>
#include <Systems/RootTransform/RootTransformSystem.h>
#include <Component/Transform.h>
#include <Component/Root.h>

using namespace ImGui;
using namespace GE::ECS;

// Anonymous namespace for functions unqiue to this file
namespace
{
	// Macros to call ecs entity name functions
#define GetName(entityId) ecs.GetEntityName(entityId).c_str() 
#define SetName(entityId, newName) ecs.SetEntityName(entityId, newName).c_str() 
#define PAYLOAD "SceneHierachy"

	// Style setting
	ImColor originalTextClr;
	ImColor inactiveTextClr{ 125, 125, 125 };

	/*!*********************************************************************
	\brief 
	  Parents the child entity to the parent entity. This function also
		adds a root node component to parent entity if it has no parent, and
		removes root node component from child if it has one.

	\param[in] ecs
		ECS context.

	\param[in] child
		Child entity.

	\param[in] parent
		Optional parent entity. Value will be NULL for no parent by default.
	************************************************************************/
	void ParentEntity(EntityComponentSystem& ecs, Entity& child, Entity* parent = nullptr);

	/*!******************************************************************
	\brief 
	  Recursive function to generate nodes for all child entities.

	\param[in] entity
		Parent entity.

	\param[in] ecs
		ECS context

	\param[in] textClr
		Colour of the text of the node
	********************************************************************/
	void Propergate(Entity entity, EntityComponentSystem& ecs, ImColor textClr);
}

void GE::EditorGUI::SceneHierachy::CreateContent()
{
	static EntityComponentSystem& ecs = EntityComponentSystem::GetInstance();
	// Get style text colour that can be edited later
	ImGuiStyle& style = GetStyle();
	originalTextClr = style.Colors[ImGuiCol_Text];

	// TODO
	// Scene should be replace with scene file name
	static const char* sceneName = "Scene";
	if (TreeNodeEx(sceneName, ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Allow user to turn an entity into a root level
		if (BeginDragDropTarget())
		{
			const ImGuiPayload* pl = AcceptDragDropPayload(PAYLOAD);
			if (pl)
			{
				GE::ECS::Entity& droppedEntity{*reinterpret_cast<GE::ECS::Entity*>(pl->Data)};
				ParentEntity(ecs, droppedEntity);
			}
			EndDragDropTarget();
		}

		// Loops through all the root nodes (highest level entities that may contain child nodes)
		for (Entity entity : ecs.GetSystem<GE::Systems::RootTransformSystem>()->GetAllEntities())
		{
			try
			{
				Propergate(entity, ecs, ecs.GetIsActiveEntity(entity) ? originalTextClr : inactiveTextClr);
			}
			catch (GE::Debug::IExceptionBase& e)
			{
				e.LogSource();
			}
		}
		TreePop();
	}

	// Reset colour
	style.Colors[ImGuiCol_Text] = originalTextClr;
}

// Anonymous namespace function definition
namespace
{
	void ParentEntity(EntityComponentSystem& ecs, Entity& child, Entity* parent)
	{
		// Child becomes root
		if (!parent)
		{
			GE::Component::Transform* childTrans = ecs.GetComponent<GE::Component::Transform>(child);

			if (!childTrans)
			{
				throw GE::Debug::Exception<GE::EditorGUI::SceneHierachy>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Node missing transform component."));
			}

			// Remove reference to child from old parent if exist
			if (childTrans->m_parent != INVALID_ID)
			{
				ecs.GetComponent<GE::Component::Transform>(childTrans->m_parent)->m_children.erase(child);
			}
			// Remove parent
			childTrans->m_parent = INVALID_ID;

			// Comvert into a root node
			GE::Component::Root root{};
			ecs.AddComponent(child, root);
			ecs.RegisterEntityToSystem<GE::Systems::RootTransformSystem>(child);
		}
		else
		{
			// Handle the case that child is a root node and has no parent
			GE::Component::Transform* childTrans = ecs.GetComponent<GE::Component::Transform>(child);
			// Remove reference of child from old parent if exist
			if (childTrans->m_parent != INVALID_ID)
			{
				ecs.GetComponent<GE::Component::Transform>(childTrans->m_parent)->m_children.erase(child);
			}
			// Remove root component if exist
			if (ecs.GetComponent<GE::Component::Root>(child))
			{
				ecs.RemoveComponent<GE::Component::Root>(child);
			}

			GE::Component::Transform* parentTrans = ecs.GetComponent<GE::Component::Transform>(*parent);

			if (!parentTrans || !childTrans)
			{
				throw GE::Debug::Exception<GE::EditorGUI::SceneHierachy>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Node missing transform component."));
			}

			// Assign child to new parent
			parentTrans->m_children.insert(child);

			
			// Assign new parent to child
			childTrans->m_parent = *parent;

		}
	}

	void Propergate(Entity entity, EntityComponentSystem& ecs, ImColor textClr)
	{
		// Abit inefficient to call get in a recursive but
		// the only other method is to decalre heap memory in a
		// static class
		ImGuiStyle& style = GetStyle();
		style.Colors[ImGuiCol_Text] = textClr;

		/////////////////////
		// Create own node
		/////////////////////
		if (TreeNodeEx(GetName(entity), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (IsItemClicked())
			{
				GE::EditorGUI::ImGuiHelper::SetSelectedEntity(entity);
			}

			////////////////////////////////////
			// Handle drag and drop orderering
			////////////////////////////////////
			if (BeginDragDropSource())
			{
				SetDragDropPayload(PAYLOAD, &entity, sizeof(entity));
				// Anything between begin and end will be parented to the dragged object
				Text(GetName(entity));
				EndDragDropSource();
			}
			if (BeginDragDropTarget())
			{
				const ImGuiPayload* pl = AcceptDragDropPayload(PAYLOAD);
				if (pl)
				{
					GE::ECS::Entity& droppedEntity{*reinterpret_cast<GE::ECS::Entity*>(pl->Data)};
					ParentEntity(ecs, droppedEntity, &entity);
				}
				EndDragDropTarget();
			}

			//////////////////////
			// Create child nodes
			//////////////////////
			GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);
			if (!trans)
			{
				// Make sure to pop newest node before exitting function
				// not sure if this doesn't crash
				TreePop();
				throw GE::Debug::Exception<GE::Systems::RootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Root node does not have transform component."));
			}

			// Recursive end condition
			if (trans->m_children.size() == 0)
			{
				// Make sure to pop newest node before exitting function
				TreePop();
				return;
			}

			for (Entity child : trans->m_children)
			{
				Propergate(child, ecs, ecs.GetIsActiveEntity(child) ? textClr : inactiveTextClr);
			}

			TreePop();
		}
	}
}