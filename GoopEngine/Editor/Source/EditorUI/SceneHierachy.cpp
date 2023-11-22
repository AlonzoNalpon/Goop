/*!*********************************************************************
\file   SceneHeirachy.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines the behaviour and logic for the scene heirachy dock
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef NO_IMGUI
#include "SceneHierachy.h"
#include <ImGui/imgui.h>
#include <Component/Transform.h>
#include <ObjectFactory/ObjectFactory.h>
#include <GameStateManager/GameStateManager.h>
#include <Systems/RootTransform/TransformSystemHelper.h>
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <ObjectFactory/SerializeComponents.h>
#include <Commands/CommandManager.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace ImGui;
using namespace GE::ECS;

// Anonymous namespace for functions unqiue to this file
namespace
{
	// Macros to call ecs entity name functions
#define GetName(entityId) ecs.GetEntityName(entityId).c_str() 
#define SetName(entityId, newName) ecs.SetEntityName(entityId, newName).c_str() 
#define PAYLOAD "ENTITY"

	// Style setting
	ImColor originalTextClr;
	ImColor inactiveTextClr{ 125, 125, 125 };

	std::vector<Entity> entitiesToDestroy;

	bool treeNodePopUp{ false };

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
	GE::GSM::GameStateManager& gsm{ GE::GSM::GameStateManager::GetInstance() };
	
	// Get style text colour that can be edited later
	ImGuiStyle& style = GetStyle();
	originalTextClr = style.Colors[ImGuiCol_Text];
	
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (TreeNodeEx(gsm.GetCurrentScene().c_str(), treeFlags))
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
	// Reset colour
	style.Colors[ImGuiCol_Text] = originalTextClr;

	ImVec2 vpSize = ImGui::GetContentRegionAvail();  // Get the top-left position of the vp
	ImVec2 vpPosition = ImGui::GetCursorScreenPos();

	if (ImGui::IsMouseHoveringRect(vpPosition, ImVec2(vpPosition.x + vpSize.x, vpPosition.y + vpSize.y)) 
		&& IsMouseClicked(ImGuiMouseButton_Right) 
		&& !treeNodePopUp)
	{
		OpenPopup("EntityCreate");
	}
	if (BeginPopup("EntityCreate"))
	{
		if (Selectable("Create"))
		{
			GE::Component::Transform trans{ {0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 } };
			GE::CMD::PRS newPRS{ trans.m_pos, trans.m_rot, trans.m_scale };
			GE::CMD::AddObjectCmd newTransCmd = GE::CMD::AddObjectCmd(newPRS);
			GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
			cmdMan.AddCommand(newTransCmd);
		}
		EndPopup();
	}

	// Delete entities after interation
	for (Entity entity : entitiesToDestroy)
	{
		//ecs.DestroyEntity(entity);
		GE::CMD::RemoveObjectCmd newRemCmd = GE::CMD::RemoveObjectCmd(entity);
		GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
		cmdMan.AddCommand(newRemCmd);
	}
	entitiesToDestroy.clear();
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
			ecs.SetParentEntity(child, INVALID_ID);

			GE::Math::dMat4 identity
			{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			};
			GE::Systems::PreRootTransformSystem::Propergate(child, identity);
		}
		else
		{
			// Checks if new parent is a child of current child
			Entity temp = *parent;
			while ((temp = ecs.GetParentEntity(temp)) != INVALID_ID)
			{
				if (temp == child)
				{
					return;
				}
			}

			try
			{
				ecs.SetParentEntity(child, *parent);
				ecs.AddChildEntity(*parent, child);

				GE::Component::Transform* parentTrans = ecs.GetComponent<GE::Component::Transform>(*parent);
				if (parentTrans == nullptr)
				{
					throw GE::Debug::Exception<GE::EditorGUI::SceneHierachy>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(*parent) + " is missing a transform component. All entities must have a transform component!!"));
				}

				GE::Systems::PreRootTransformSystem::Propergate(child, parentTrans->m_worldTransform);
			}
			catch (GE::Debug::IExceptionBase& e)
			{
				e.LogSource();
			}
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
		std::set<Entity>& m_children = ecs.GetChildEntities(entity);
		if (m_children.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}

		if (TreeNodeEx(GetName(entity), flag |
			(entity == GE::EditorGUI::ImGuiHelper::GetSelectedEntity() ? ImGuiTreeNodeFlags_Selected : 0)))
		{
			if (IsItemClicked())
			{
				GE::EditorGUI::ImGuiHelper::SetSelectedEntity(entity);				
			}

			PushID(std::to_string(entity).c_str());
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				OpenPopup("EntityManip");
				treeNodePopUp = true;
			}
			if (BeginPopup("EntityManip"))
			{
				if (Selectable("Create"))
				{
					Entity newEntity = ecs.CreateEntity();
					GE::Component::Transform trans{{0, 0, 0}, { 1, 1, 1 }, { 0, 0, 0 }};
					ecs.AddComponent(newEntity, trans);
				}

				if (Selectable("Duplicate"))
				{
					GE::ObjectFactory::ObjectFactory::GetInstance().CloneObject(entity);
				}

				if (Selectable("Delete"))
				{
					entitiesToDestroy.push_back(entity);
				}
				EndPopup();
			}
			else
			{
				treeNodePopUp = false;
			}
			PopID();

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
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_IMAGE"))
				{
					if (payload->Data)
					{
						auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
						const char* droppedPath = static_cast<const char*>(payload->Data);
						std::string extension = GE::GoopUtils::GetFileExtension(droppedPath);
						if (ecs.HasComponent<GE::Component::Sprite>(entity))
						{
							GE::Component::Sprite* entitySpriteData = ecs.GetComponent<GE::Component::Sprite>(entity);
							entitySpriteData->m_spriteData.texture = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath));
						}
						else
						{
							auto& gEngine = Graphics::GraphicsEngine::GetInstance();
							GE::Component::Sprite sprite{ gEngine.textureManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath)) };
							ecs.AddComponent(entity, sprite);
						}
					}
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_FONT"))
				{
					if (payload->Data)
					{
						auto& gEngine = Graphics::GraphicsEngine::GetInstance();

						//auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
						const char* droppedPath = static_cast<const char*>(payload->Data);
						std::string extension = GE::GoopUtils::GetFileExtension(droppedPath);
						if (ecs.HasComponent<GE::Component::Text>(entity))
						{
							// Entity has text component, replacing the font instead
							GE::Component::Text* entityTextData = ecs.GetComponent<GE::Component::Text>(entity);
							entityTextData->m_fontID = gEngine.fontManager.GetFontID(GE::GoopUtils::ExtractFilename(droppedPath));
						}
						else
						{
							// Entity does not have text component, adding a text component and assigning the dropped font.
							GE::Component::Text comp;
							comp.m_fontID = gEngine.fontManager.GetFontID(GE::GoopUtils::ExtractFilename(droppedPath));
							ecs.AddComponent(entity, comp);
						}
					}
				}

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
			if (m_children.empty())
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
		else
		{
			if (IsItemClicked())
			{
				GE::EditorGUI::ImGuiHelper::SetSelectedEntity(entity);
			}
		}
	}
}
#endif