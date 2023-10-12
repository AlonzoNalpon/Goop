/*!******************************************************************
\file   SceneHeirachy.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines the behaviour and logic for the scene heirachy dock
********************************************************************/
#include <pch.h>
#include "SceneHeirachy.h"
#include <ImGui/imgui.h>
#include <Systems/RootTransform/RootTransformSystem.h>
#include <Component/Transform.h>

using namespace ImGui;
using namespace GE::ECS;

// Anonymous namespace for functions unqiue to this file
namespace
{
	// Macros to call ecs entity name functions
#define GetName(entityId) ecs.GetEntityName(entityId).c_str() 
#define SetName(entityId, newName) ecs.SetEntityName(entityId, newName).c_str() 

	/*!******************************************************************
	\brief 
	  Recursive function to generate nodes for all child entities,

	\param[in] entity
		Parent entity.

	\param[in] ecs
		ECS context
	********************************************************************/
	void Propergate(Entity entity, EntityComponentSystem& ecs)
	{
		GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);
		if (!trans)
		{
			throw GE::Debug::Exception<GE::Systems::RootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Root node does not have transform component."));
		}

		// Recursive end condition
		if (trans->m_children.size() == 0)
		{
			return;
		}

		for (Entity child : trans->m_children)
		{
			if (TreeNodeEx(GetName(child), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
			{
				Propergate(child, ecs);
				TreePop();
			}
		}
	}
}

void GE::EditorGUI::SceneHeirachy::CreateContent()
{
	static EntityComponentSystem& ecs = EntityComponentSystem::GetInstance();

	ShowDemoWindow();

	// Loops through all the root nodes (highest level entities that may contain child nodes)
	for (Entity entity : ecs.GetSystem<GE::Systems::RootTransformSystem>()->GetEntities())
	{
		// Root node (Has children)
		if (TreeNodeEx(GetName(entity), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
		{
			Propergate(entity, ecs);
			TreePop();
		}
	}
}
