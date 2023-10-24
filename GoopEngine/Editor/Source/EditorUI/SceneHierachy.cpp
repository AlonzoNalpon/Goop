/*!*********************************************************************
\file   SceneHeirachy.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines the behaviour and logic for the scene heirachy dock
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "SceneHierachy.h"
#include <ImGui/imgui.h>
#include <Systems/RootTransform/RootTransformSystem.h>
#include <Component/Transform.h>
#include "../ObjectFactory/ObjectFactory.h"

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

	std::vector<Entity> entitiesToDestroy;

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
	void Propergate(Entity entity, EntityComponentSystem& ecs, ImGuiTreeNodeFlags flag, ImColor textClr);
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
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (TreeNodeEx(sceneName, treeFlags))
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

		for (Entity entity : ecs.GetEntities())
		{
			if (ecs.GetParentEntity(entity) == INVALID_ID)
			{
				Propergate(entity, ecs, treeFlags, ecs.GetIsActiveEntity(entity) ? originalTextClr : inactiveTextClr);
			}
		}
		TreePop();
	}

	for (Entity entity : entitiesToDestroy)
	{
		ecs.DestroyEntity(entity);
	}
	entitiesToDestroy.clear();

	// Reset colour
	style.Colors[ImGuiCol_Text] = originalTextClr;
}

// Anonymous namespace function definition
namespace
{
	void ParentEntity(EntityComponentSystem& ecs, Entity& child, Entity* parent)
	{
		Entity oldParent = ecs.GetParentEntity(child);
		// Has parent, remove self from parent
		if (oldParent != INVALID_ID)
		{
			ecs.RemoveChildEntity(oldParent, child);
		}

		if (!parent)	// Child becoming root
		{
			if (oldParent != INVALID_ID)
			{
				GE::Component::Transform& childTrans = *ecs.GetComponent<GE::Component::Transform>(child);
				GE::Component::Transform& parentTrans = *ecs.GetComponent<GE::Component::Transform>(oldParent);

				childTrans.m_pos = childTrans.m_parentWorldTransform * GE::Math::dVec4(childTrans.m_pos, 1.0);
				childTrans.m_scale = { childTrans.m_scale.x * parentTrans.m_scale.x,childTrans.m_scale.y * parentTrans.m_scale.y, 1.0 };
				childTrans.m_rot = childTrans.m_rot + parentTrans.m_rot;
			}

			// Remove reference to child from old parent if exist
			ecs.SetParentEntity(child, INVALID_ID);
		}
		else
		{
			GE::Component::Transform& childTrans = *ecs.GetComponent<GE::Component::Transform>(child);
			GE::Component::Transform& parentTrans = *ecs.GetComponent<GE::Component::Transform>(*parent);

			GE::Math::dMat4 invP;
			GE::Math::MtxInverse(invP, parentTrans.m_worldTransform);
			childTrans.m_pos = invP * GE::Math::dVec4(childTrans.m_pos, 1.0);
			childTrans.m_scale = { childTrans.m_scale.x / parentTrans.m_scale.x,childTrans.m_scale.y / parentTrans.m_scale.y, 1.0 };
			childTrans.m_rot = childTrans.m_rot - parentTrans.m_rot;

			ecs.SetParentEntity(child, *parent);
		  ecs.AddChildEntity(*parent, child);
		}
	}

	void Propergate(Entity entity, EntityComponentSystem& ecs, ImGuiTreeNodeFlags flag, ImColor textClr)
	{
		// Abit inefficient to call get in a recursive but
		// the only other method is to decalre heap memory in a
		// static class
		ImGuiStyle& style = GetStyle();
		style.Colors[ImGuiCol_Text] = textClr;

		/////////////////////
		// Create own node
		/////////////////////
		if (TreeNodeEx(GetName(entity), flag))
		{
			if (IsItemClicked())
			{
				GE::EditorGUI::ImGuiHelper::SetSelectedEntity(entity);
			}
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				OpenPopup("EntityManip");
			}
			if (BeginPopup("EntityManip"))
			{
				if (Selectable("Duplicate"))
				{
					GE::ObjectFactory::ObjectFactory::GetInstance().CloneObject(entity, ecs.GetComponent<GE::Component::Transform>(entity)->m_pos);
				}

				if (Selectable("Delete"))
				{
					entitiesToDestroy.push_back(entity);
				}
				EndPopup();
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
			// Recursive end condition
			std::vector<Entity>& m_children = ecs.GetChildEntities(entity);
			if (m_children.size() == 0)
			{
				// Make sure to pop newest node before exitting function
				TreePop();
				return;
			}

			for (Entity child : m_children)
			{
				Propergate(child, ecs, flag, ecs.GetIsActiveEntity(child) ? textClr : inactiveTextClr);
			}

			TreePop();
		}
	}
}