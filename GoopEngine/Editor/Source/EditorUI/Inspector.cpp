/*!*********************************************************************
\file   Inspector.cpp
\author w.chinkitbryam\@digipen.edu
\co-authors a.nalpon\@digipen.edu (42 lines)
						loh.j\@digipen.edu (14 lines)
\date   23 October 2023
\brief
	Wrapper class to create an EditorGUI for the tool bar

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "ImGuiUI.h"
#include "Inspector.h"
#include <ImGui/imgui.h>
#include <Component/Components.h>
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Commands/CommandManager.h>
#include <Graphics/GraphicsEngine.h>
#include <EditorUI/GizmoEditor.h>
#include <ImNode/NodeEditor.h>
#include <rttr/type.h>
#include <Systems/Button/ButtonTypes.h>
#include <GameDef.h>
#include <Prefabs/PrefabManager.h>
#include "PrefabEditor.h"
#include <Events/EventManager.h>
#include <ScriptEngine/CSharpStructs.h>
#include <Events/AnimEventManager.h>

// Disable empty control statement warning
#pragma warning(disable : 4390)
// Disable reinterpret to larger size
#pragma warning(disable : 4312)

using namespace ImGui;
using namespace GE::Component;

namespace
{
	/*!*********************************************************************
	\brief 
		Wrapper to create a vector3 input field for doubles

	\param[in] propertyName
		Label name

	\param[in,out] property
		Vec3 input

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled

	\return
		Field changed
	************************************************************************/
	bool InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled = false);

	/*!*********************************************************************
	\brief
		Wrapper to create a input field for double

	\param[in] propertyName
		Label name

	\param[in,out] property
		Double input

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	void InputDouble1(std::string propertyName, double& property, bool disabled = false);

	/*!*********************************************************************
	\brief
		Wrapper to create a checkbox

	\param[in] propertyName
		Label name

	\param[in] property
		Bool input

	\param[in,out] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled

	\return
		If value has changed
	************************************************************************/
	bool InputCheckBox(std::string propertyName, bool& property, bool disabled = false);

	/*!******************************************************************
	\brief
	  Wrapper to create an input field for an entity reference.

	\param[in] propertyName
		Label name

	\param[in,out] entity
		Input entity

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	********************************************************************/
	bool InputEntity(std::string propertyName, GE::ECS::Entity& entity, bool disabled = false);

	/*!*********************************************************************
	\brief
		This function is not defined. Only used for specialization.

	\param[in] propertyName
		Label name

	\param[in] list
		List input

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <typename Container>
	void InputList(std::string propertyName, Container& list, float fieldWidth, bool disabled = false);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of vector of 
		linear forces

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of linear forces

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of deque of
		vec3

	\param[in] propertyName
		Label name

	\param[in] list
		Deque of vec3

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::deque<GE::Math::dVec3>& list, float fieldWidth, bool disabled);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of queue of
		Tweens

	\param[in] propertyName
		Label name

	\param[in] list
		Queue of Tweens

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::map<std::string, std::vector<GE::Component::Tween::Action>>& list, float fieldWidth, bool disabled);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of vector of
		Entities

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of entities

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::vector<GE::ECS::Entity>& list, float fieldWidth, bool disabled);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of vector of
		Entities

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of ints

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::vector<int>& list, float fieldWidth, bool disabled);

	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of vector of
		sounds in Audio component

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of audio sounds

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	template <>
	void InputList(std::string propertyName, std::vector<GE::Component::Audio::Sound>& list, float fieldWidth, bool disabled);


	/*!*********************************************************************
	\brief
		Wrapper to create specialized inspector list of vector of integers.
		This function is specifically for diaplying script fields/data members

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of int values

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	bool InputScriptList(std::string propertyName, std::vector<int>& list, float fieldWidth, bool disabled = false);

	/*!*********************************************************************
	\brief
			Wrapper to create specialized inspector list of vector of unsigned int.
		This function is specifically for diaplying script fields/data members

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of unsigned int

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	bool  InputScriptList(std::string propertyName, std::vector<unsigned>& list, float fieldWidth, bool disabled = false);

	/*!*********************************************************************
	\brief
			Wrapper to create specialized inspector list of vector of unsigned int.
		This function is specifically for diaplying script fields/data members

	\param[in] propertyName
		Label name

	\param[in] list
		Vector of unsigned int

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled
	************************************************************************/
	bool  InputCardID(std::string propertyName, std::vector<Card::CardID>& list, float fieldWidth, bool disabled = false);

	/*!*********************************************************************
	\brief
		Predefined behaviour of a remove component popup

	\param[in] name
		Name of the popup

	\return
		If component was removed
	************************************************************************/
	template <typename T>
	bool RemoveComponentPopup(std::string name, GE::ECS::Entity entity);
}

void GE::EditorGUI::Inspector::CreateContent()
{
	GE::ECS::Entity entity = ImGuiHelper::GetSelectedEntity();
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

	if (entity == GE::ECS::INVALID_ID || !ecs.GetEntities().contains(entity))
		return;

	GE::ECS::ComponentSignature sig = ecs.GetComponentSignature(entity);

	ImGui::PushID(entity);
	bool isActive{ecs.GetIsActiveEntity(entity)};
	if (Checkbox("##IsActive", &isActive))
	{
		ecs.SetIsActiveEntity(entity, isActive);
	}
	ImGui::SameLine();
	std::string name = ecs.GetEntityName(entity);

	bool const canRename{ PrefabEditor::IsEditingPrefab() && PrefabEditor::GetCurrentEntity() == entity };
	ImGui::BeginDisabled(canRename);
	if (InputText("##Name", &name))
	{
		ecs.SetEntityName(entity, name);
	}
	ImGui::EndDisabled();
	ImGui::Text(("Entity ID: " + std::to_string(entity)).c_str());

	if (!PrefabEditor::IsEditingPrefab())
	{
		auto prefabSource{ Prefabs::PrefabManager::GetInstance().GetEntityPrefab(entity) };
		if (prefabSource)
		{
			auto& prefab{ prefabSource->get() };
			ImGui::Text(("Prefab Source: " + prefab.m_prefab).c_str());
			ImGui::Text("Receive Prefab Updates");
			ImGui::SameLine();
			ImGui::Checkbox("##PrefabUpdates", &prefab.m_registered);
		}
	}

	GizmoEditor::SetVisible(false);
	for (unsigned i{}; i < ECS::componentTypes.size(); ++i)
	{
		if (sig[i])
		{
			float contentSize = GetWindowSize().x;
			// 15 characters for property name
			float charSize = CalcTextSize("012345678901234").x;

			static bool tweenPopUp = false;

			rttr::type const& compType{ ECS::componentTypes[i] };
			if (compType == rttr::type::get<Component::Transform>())
			{
				auto trans = ecs.GetComponent<Transform>(entity);
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					// Honestly no idea why -30 makes all 3 input fields match in size but sure
					float inputWidth = (contentSize - charSize - 30) / 3;
					GE::CMD::PRS oldPRS{ trans->m_pos, trans->m_rot, trans->m_scale };
					bool valChanged{ false };

					ImGui::Separator();
					// SET GIZMO RADIO BUTTONS
					{
						// SET MODE BASED ON KEYBINDS: W,E,R (trans, rot, scale)
						{
							if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_W))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::TRANSLATE);
							if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::ROTATE_Z);
							if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::SCALE);
						}
						// TRANSLATE
						{
							if (ImGui::RadioButton("Translate",
								GizmoEditor::GetCurrOperation() == ImGuizmo::OPERATION::TRANSLATE))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::TRANSLATE);
						}
						ImGui::SameLine();
						// ROTATE
						{
							if (ImGui::RadioButton("Rotate",
								GizmoEditor::GetCurrOperation() == ImGuizmo::OPERATION::ROTATE_Z))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::ROTATE_Z);
						}
						ImGui::SameLine();
						// SCALE
						{
							if (ImGui::RadioButton("Scale",
								GizmoEditor::GetCurrOperation() == ImGuizmo::OPERATION::SCALE))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::SCALE);
						}
						ImGui::SameLine();
					}


					ImGui::Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					ImGui::TableNextRow();
					if (InputDouble3("Position", trans->m_pos, inputWidth)) { valChanged = true; };
					ImGui::TableNextRow();
					if (InputDouble3("Scale", trans->m_scale, inputWidth)) { valChanged = true; };
					ImGui::TableNextRow();
					if (InputDouble3("Rotation", trans->m_rot, inputWidth)) { valChanged = true; };

					TableNextRow();
					InputDouble3("World Position", trans->m_worldPos, inputWidth, true);
					TableNextRow();
					InputDouble3("World Scale", trans->m_worldScale, inputWidth, true);
					TableNextRow();
					InputDouble3("World Rotation", trans->m_worldRot, inputWidth, true);

					EndTable();
					ImGui::Separator();
					if (valChanged) {
						GizmoEditor::SetVisible(false);
						GE::CMD::ChangeTransCmd newTransCmd = GE::CMD::ChangeTransCmd(oldPRS, { trans->m_pos, trans->m_rot, trans->m_scale }, entity);
						GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
						cmdMan.AddCommand(newTransCmd);
					}
					else if (trans->m_scale.x && trans->m_scale.y && trans->m_scale.z)
					{
						GizmoEditor::SetVisible(true);
						GizmoEditor::SetCurrentObject(trans->m_worldTransform, entity);
					}
				}
			}
			else if (compType == rttr::type::get<Component::BoxCollider>())
			{
				auto col = ecs.GetComponent<BoxCollider>(entity);
				if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<BoxCollider>("Collider", entity))
					{
						break;
					}

					ImGui::Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Width", col->m_width);
					TableNextRow();
					InputDouble1("Height", col->m_height);
					TableNextRow();
#ifndef IMGUI_DISABLE
					InputCheckBox("Show Collider", col->m_render);
#endif
					if (Button("Match Sprite Unscaled"))
					{
						auto* sprite = ecs.GetComponent<Sprite>(entity);
						if (sprite)
						{
							col->m_width = sprite->m_spriteData.info.width;
							col->m_height = sprite->m_spriteData.info.height;
						}
					}

					if (Button("Match Sprite And Scale"))
					{
						auto* sprite = ecs.GetComponent<Sprite>(entity);
						auto* trans = ecs.GetComponent<Transform>(entity);
						if (sprite)
						{
							col->m_width = sprite->m_spriteData.info.width * trans->m_scale.x;
							col->m_height = sprite->m_spriteData.info.height * trans->m_scale.y;
						}
					}
					EndTable();
					ImGui::Separator();
				}
			}
			else if (compType == rttr::type::get<Component::Velocity>())
			{
				auto vel = ecs.GetComponent<Velocity>(entity);
				if (ImGui::CollapsingHeader("Forces", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Velocity>("Forces", entity))
					{
						break;
					}

					// Honestly no idea why -30 makes all 3 input fields match in size but sure
					float inputWidth = (contentSize - charSize - 30) / 3;

					Separator();
					PushID("Forces");
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble3("Velocity", vel->m_vel, inputWidth);
					TableNextRow();
					InputDouble3("Accelaration", vel->m_acc, inputWidth);
					ImGui::TableNextRow();
					InputDouble1("Mass", vel->m_mass);
					ImGui::TableNextRow();
					InputDouble3("Gravity", vel->m_gravity, inputWidth);		
					ImGui::TableNextRow();
					InputDouble1("Drag", vel->m_dragForce.m_magnitude);
					ImGui::TableNextRow();
					InputCheckBox("Drag Active", vel->m_dragForce.m_isActive);
					EndTable();
					InputList("Linear Forces", vel->m_linearForces, inputWidth);
					ImGui::Separator();
					/*InputList("Impulse Forces", vel->m_impulseForces, inputWidth);
					ImGui::Separator();*/
					ImGui::PopID();
				}
			}
			else if (compType == rttr::type::get<Component::Sprite>())
			{
				bool hasSpriteAnim = ecs.HasComponent<SpriteAnim>(entity);
				auto* sprite = ecs.GetComponent<Sprite>(entity);
				if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Sprite>("Sprite", entity))
					{
						break;
					}
					ImVec2 imgSize{ 100, 100 };
					SetCursorPosX(GetContentRegionAvail().x / 2 - imgSize.x / 2);
					ImageButton(reinterpret_cast<ImTextureID>(sprite->m_spriteData.texture), imgSize, { 0, 1 }, { 1, 0 });
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_IMAGE"))
						{
							if (payload->Data)
							{
								auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
								const char* droppedPath = static_cast<const char*>(payload->Data);
								*sprite = Sprite(texManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath)));
							}
							EndDragDropTarget();
						}
					}
					//ImGui::Columns(1);

					if (hasSpriteAnim) // If there's a sprite anim component, we shouldn't be able to edit
					{
						TextColored({ 1.f, 0.f, 0.f, 1.f }, "SpriteAnim detected! Unable to edit!");
						BeginDisabled();
					}
#pragma region SPRITE_LIST
					auto spriteObj = ecs.GetComponent<Component::Sprite>(entity);
					Separator();
					BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);

					TableNextColumn();

					auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
					auto const& textureLT{ textureManager.GetTextureLT() };
					if (BeginCombo("Sprite", textureManager.GetTextureName(spriteObj->m_spriteData.texture).c_str()))
					{
						for (auto const& it : textureLT)
						{
							if (Selectable(it.first.c_str()))
							{
								auto const& texture{ textureManager.GetTexture(it.second) };
								spriteObj->m_spriteName = it.first;
								spriteObj->m_spriteData.texture = texture.textureHandle;
								spriteObj->m_spriteData.info.height = texture.height;
								spriteObj->m_spriteData.info.width = texture.width;
								spriteObj->m_spriteData.info.texDims = { 1.f, 1.f }; // default
								spriteObj->m_spriteData.info.texCoords = {}; // bottom left
							}
						}
						EndCombo();
					}
#pragma endregion
#pragma region SPRITE_DEBUG_INFO 
					int imageDims[2]{ static_cast<int>(spriteObj->m_spriteData.info.width), 
														static_cast<int>(spriteObj->m_spriteData.info.height) };
					if (ImGui::InputInt("Image Width", &imageDims[0]))
						imageDims[0] = imageDims[0] < 0 ? 0 : imageDims[0];
					if (ImGui::InputInt("Image Height", &imageDims[1]))
						imageDims[1] = imageDims[1] < 0 ? 0 : imageDims[1];
					
					spriteObj->m_spriteData.info.width = static_cast<GLuint>(imageDims[0]);
					spriteObj->m_spriteData.info.height = static_cast<GLuint>(imageDims[1]);
					ImGui::InputFloat2("Tex Coords", &spriteObj->m_spriteData.info.texCoords.r);
					ImGui::InputFloat2("Tex Dims", &spriteObj->m_spriteData.info.texDims.x);
					if (ImGui::Button("Reset"))
					{
						auto const& texture{ textureManager.GetTexture(spriteObj->m_spriteData.texture) };
						// Name's not necessary. Remove in the future if still not needed.
						spriteObj->m_spriteName = textureManager.GetTextureName(spriteObj->m_spriteData.texture);
						spriteObj->m_spriteData.texture = texture.textureHandle;
						spriteObj->m_spriteData.info.height = texture.height;
						spriteObj->m_spriteData.info.width = texture.width;
						spriteObj->m_spriteData.info.texDims = { 1.f, 1.f }; // default
						spriteObj->m_spriteData.info.texCoords = {}; // bottom left
					}
					ImGui::SameLine();
					if (ImGui::Button("Force Width To Match Tex AR"))
					{
						auto const& texture{ textureManager.GetTexture(spriteObj->m_spriteData.texture) };
						double ar = static_cast<double>(texture.width) / texture.height;
						spriteObj->m_spriteData.info.width = static_cast<GLint>(spriteObj->m_spriteData.info.height * ar);
					}

					if (hasSpriteAnim)
						EndDisabled();
					EndTable();
					Separator();
#pragma endregion
				}
			}
			else if (compType == rttr::type::get<Component::SpriteAnim>())
			{
				//auto anim = ecs.GetComponent<SpriteAnim>(entity);
				if (ImGui::CollapsingHeader("Sprite Animation", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<SpriteAnim>("Sprite Animation", entity))
					{
						break;
					}
#pragma region SPRITE_ANIM_LIST
					auto spriteAnimObj = ecs.GetComponent<Component::SpriteAnim>(entity);
					Separator();
					BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);

					TableNextColumn();

					auto const& animManager{ Graphics::GraphicsEngine::GetInstance().animManager };
					auto const& textureLT{ animManager.GetAnimLT()};
					if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj->animID).c_str()))
					{
						for (auto const& it : textureLT)
						{
							if (Selectable(it.first.c_str()))
							{
								auto spriteObj = ecs.GetComponent<Component::Sprite>(entity);
								auto const& anim = animManager.GetAnim(it.second);
								Systems::SpriteAnimSystem::SetAnimation(entity, it.second);

								// setting correct attributes for sprite
								auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };

								// Set sprite name and the texture handle. This is for rendering in editor
								spriteObj->m_spriteName = textureManager.GetTextureName(anim.texture);
								spriteObj->m_spriteData.texture = anim.texture;
								spriteObj->m_spriteData.info = anim.frames[0]; // and set actual sprite info
							}
						}
						EndCombo();
					}
					// Display animation ID for users to know
					TextColored({ 1.f, .7333f, 0.f, 1.f }, ("Animation ID: " + std::to_string(spriteAnimObj->animID)).c_str());

					EndTable();
					ImGui::Separator();
#pragma endregion
				}
			}
			else if (compType == rttr::type::get<Component::Tween>())
			{
				// Honestly no idea why -30 makes all 3 input fields match in size but sure
				float inputWidth = (contentSize - charSize - 30) / 3;

				auto tween = ecs.GetComponent<Tween>(entity);
				if (ImGui::CollapsingHeader("Tween", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Tween>("Tween", entity))
					{
						break;
					}

					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH);
					TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Time Elapsed", tween->m_timeElapsed);
					TableNextRow();
					InputDouble3("Last End Point", tween->m_originalPos, inputWidth);
					TableNextRow();
					InputCheckBox("Paused", tween->m_paused);
					TableNextRow();
					InputCheckBox("Loop", tween->m_loop);
					TableNextRow();
					TableNextColumn();
					ImGui::Text("Playing Anim");
					TableNextColumn();
					InputText("##", &tween->m_playing);					
					EndTable();
					InputList("", tween->m_tweens, inputWidth);
					if (Button("Add Tween Animation", { GetContentRegionMax().x, 20 }))
					{
						OpenPopup("Add Tween Animation");
					}

					static bool invalidName = false;
					static bool blankName = false;
					static std::string tweenName;
					SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (BeginPopupModal("Add Tween Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						if (invalidName)
						{
							TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tween already exist!");
						}
						if (blankName)
						{
							TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tween name cannot be blank!");
						}

						ImGui::Text("Animation Name");
						SameLine();
						if (ImGui::InputText("##", &tweenName))
						{
							invalidName = false;
							blankName = false;
						}

						SetCursorPosX(0.5f * (ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Cancel Create ").x));
						if (Button("Cancel"))
						{
							invalidName = false;
							blankName = false;
							tweenName.clear();
							CloseCurrentPopup();
						}
						SameLine();
						if (Button("Create"))
						{
							if (tween->m_tweens.find(tweenName) != tween->m_tweens.end())
							{
								invalidName = true;
							}
							else if (tweenName == "")
							{
								blankName = true;
							}
							else
							{
								invalidName = false;
								blankName = false;
								tween->AddTween(tweenName, { {0, 0, 0}, {1, 1, 1}, {0, 0, 0}, 1 });
								tweenName.clear();
								CloseCurrentPopup();
							}
						}

						EndPopup();
					}

					//NewTweenActionPopUp(*tween);
					ImGui::Separator();
				}

			}
			else if (compType == rttr::type::get<Component::EnemyAI>())
			{

				if (ImGui::CollapsingHeader("EnemyAI", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const std::vector<std::pair<DisplayTree, NodeLinkList>>& treeList = GE::AI::NodeEditor::GetInstance().GetTreeList();
					auto aiComp = ecs.GetComponent<EnemyAI>(entity);
					auto iter = std::find_if(treeList.begin(), treeList.end(), [aiComp](const std::pair<DisplayTree, NodeLinkList> & tree) -> bool
						{
							return tree.first.m_treeID == aiComp->m_treeID;
						});
					std::string defName = (iter != treeList.end()) ? iter->first.m_treeName : "";

					if (ImGui::BeginCombo("##TreeNames", defName.c_str()))
					{
						for (const std::pair<DisplayTree, NodeLinkList>& tree: treeList)
						{
							if (tree.first.m_treeID != aiComp->m_treeID)
							{
								bool is_selected{ false };
								if (ImGui::Selectable(tree.first.m_treeName.c_str(), &is_selected))
								{
									aiComp->m_treeID = tree.first.m_treeID;
									aiComp->RefreshCache();
								}
								if (is_selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}
						}
						ImGui::EndCombo();
					}
				}
			}
			else if (compType == rttr::type::get<Component::Scripts>())
			{
				//auto scripts = ecs.GetComponent<Scripts>(entity);
				if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float inputWidth = (contentSize - charSize - 30) / 3;
					std::vector <std::string> toDeleteList{};

					GE::Component::Scripts* allScripts = ecs.GetComponent<Scripts>(entity);
					if (RemoveComponentPopup<Scripts>("Script", entity))
					{
						break;
					}
					GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
					for (ScriptInstance& s : allScripts->m_scriptList)
					{
						s.GetAllUpdatedFields();
						ImGui::Separator();
						BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
						ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);

						TableNextRow();
						BeginDisabled(false);
						TableNextColumn();
						ImGui::Text("Script");
						TableNextColumn();
						ImGuiStyle& style = GetStyle();
						ImVec4 originalColor = style.Colors[ImGuiCol_FrameBg];
						ImVec4 originalHColor = style.Colors[ImGuiCol_FrameBgHovered];
						style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.28f, 0.66f, 1.0f);
						style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.48f, 0.86f, 1.0f);
						if (ImGui::BeginCombo("", s.m_scriptName.c_str()))
						{
							for (const std::string& sn : sm->m_allScriptNames)
							{
								auto it = std::find_if(allScripts->m_scriptList.begin(), allScripts->m_scriptList.end(), [sn](const ScriptInstance pair) { return pair.m_scriptName == sn; });
								if (it == allScripts->m_scriptList.end())
								{
									bool is_selected = (s.m_scriptName.c_str() == sn);
									if (ImGui::Selectable(sn.c_str(), is_selected))
									{
										if (sn != s.m_scriptName) {
											s = ScriptInstance(sn, entity);
										}
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						ImVec4 boriginalColor = style.Colors[ImGuiCol_Button];
						ImVec4 boriginalHColor = style.Colors[ImGuiCol_ButtonHovered];
						ImVec4 boriginalAColor = style.Colors[ImGuiCol_ButtonActive];
						style.Colors[ImGuiCol_Button] = ImVec4(0.6f, 0.f, 0.29f, 1.0f);
						style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.1f, 0.49f, 1.0f);
						style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.3f, 0.39f, 1.0f);
						if(ImGui::Button("Delete"))
						{
							toDeleteList.push_back(s.m_scriptName);
						}
						style.Colors[ImGuiCol_Button] = boriginalColor;
						style.Colors[ImGuiCol_ButtonHovered] = boriginalHColor;
						style.Colors[ImGuiCol_ButtonActive] = boriginalAColor;
						EndDisabled();
						style.Colors[ImGuiCol_FrameBg] = originalColor;
						style.Colors[ImGuiCol_FrameBgHovered] = originalHColor;

						// Gets all the script field/data member from the vector of rttr:variant and display them inside the inspector
						for (rttr::variant& f  : s.m_scriptFieldInstList)
						{
							rttr::type dataType{ f.get_type() };
							// get underlying type if it's wrapped in a pointer
							if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<int>>())
							{
								TableNextRow();
								BeginDisabled(false);
								GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
								TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								ImGui::TableNextColumn();
								SetNextItemWidth(GetWindowSize().x);
								if (ImGui::InputInt(("##" + sfi.m_scriptField.m_fieldName).c_str(), &(sfi.m_data), 0, 0, 0)) { s.SetFieldValue<int>(sfi.m_data,sfi.m_scriptField.m_classField ); }
								EndDisabled();

							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())
							{
								TableNextRow();
								BeginDisabled(false);
								GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
								ImGui::TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								ImGui::TableNextColumn();
								SetNextItemWidth(GetWindowSize().x);
								if (ImGui::InputFloat(("##" + sfi.m_scriptField.m_fieldName).c_str(), &(sfi.m_data), 0, 0, 0)) { s.SetFieldValue<float>(sfi.m_data, sfi.m_scriptField.m_classField); }
								EndDisabled();
							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<double>>())
							{
								TableNextRow();
								BeginDisabled(false);
								GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
								TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								ImGui::TableNextColumn();
								SetNextItemWidth(GetWindowSize().x);
								if (ImGui::InputDouble(("##" + sfi.m_scriptField.m_fieldName).c_str(), &(sfi.m_data), 0, 0, 0)) { s.SetFieldValue<double>(sfi.m_data, sfi.m_scriptField.m_classField); }
								EndDisabled();
							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>())
							{
								TableNextRow();
								GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
								if (InputDouble3(("##" + sfi.m_scriptField.m_fieldName).c_str(), sfi.m_data, inputWidth)) { s.SetFieldValue<GE::Math::dVec3>(sfi.m_data, sfi.m_scriptField.m_classField); };
							}

							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<DeckManager>>())
							{
								TableNextRow();
								BeginDisabled(false);
								GE::MONO::ScriptFieldInstance<DeckManager>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<DeckManager>>();
								GE::MONO::ScriptInstance& deck = sfi.m_data.m_deckInstance;
								GE::MONO::ScriptInstance& deckMan = sfi.m_data.m_deckManagerInstance;

								for (rttr::variant& dF : deck.m_scriptFieldInstList)
								{
									
									ScriptFieldInstance<std::vector<Component::Card::CardID>>& dSFI = dF.get_value<ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
									//std::cout << dSFI.m_scriptField.m_fieldName.c_str() << ":: " << dSFI.m_data.size() << "\n";
									TableNextColumn();
									ImGui::Text(dSFI.m_scriptField.m_fieldName.c_str());
									ImGui::TableNextColumn();
									if (InputCardID("##" + dSFI.m_scriptField.m_fieldName, dSFI.m_data, inputWidth))
									{
										deck.SetFieldValueArr<Component::Card::CardID>(dSFI.m_data, sm->m_appDomain, dSFI.m_scriptField.m_classField);
										if (dSFI.m_scriptField.m_fieldName == "m_cards")
											sfi.m_data.m_deck.m_cards = dSFI.m_data;
										if (dSFI.m_scriptField.m_fieldName == "m_drawOrderDisplay")
											sfi.m_data.m_deck.m_drawOrderDisplay = dSFI.m_data;
									}
								
								}

								for (rttr::variant& dF : deckMan.m_scriptFieldInstList)
								{
									ScriptFieldInstance<std::vector<Component::Card::CardID>>& dSFI = dF.get_value<ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
									TableNextColumn();
									ImGui::Text(dSFI.m_scriptField.m_fieldName.c_str());
									ImGui::TableNextColumn();
									if (InputCardID("##" + dSFI.m_scriptField.m_fieldName, dSFI.m_data, inputWidth))
									{
										deckMan.SetFieldValueArr<Component::Card::CardID>(dSFI.m_data, sm->m_appDomain, dSFI.m_scriptField.m_classField);
										if (dSFI.m_scriptField.m_fieldName == "m_hand")
											sfi.m_data.m_hand = dSFI.m_data;
										if (dSFI.m_scriptField.m_fieldName == "m_queue")
											sfi.m_data.m_queue = dSFI.m_data;
										if (dSFI.m_scriptField.m_fieldName == "m_discardDisplay")
											sfi.m_data.m_discardDisplay = dSFI.m_data;
									}
								
								}


								EndDisabled();
							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<HealthBar>>())
							{
								TableNextRow();
								BeginDisabled(false);
								GE::MONO::ScriptFieldInstance<HealthBar>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<HealthBar>>();
								GE::MONO::ScriptInstance& healthBar = sfi.m_data.m_HealthBarInst;

								for (rttr::variant& dF : healthBar.m_scriptFieldInstList)
								{

									ScriptFieldInstance<int>& dSFI = dF.get_value<ScriptFieldInstance<int>>();
									//std::cout << dSFI.m_scriptField.m_fieldName.c_str() << ":: " << dSFI.m_data.size() << "\n";
									TableNextColumn();
									ImGui::Text(dSFI.m_scriptField.m_fieldName.c_str());
									ImGui::TableNextColumn();
									if (ImGui::InputInt(("##" + dSFI.m_scriptField.m_fieldName).c_str(), &(dSFI.m_data), 0, 0, 0))
									{
										healthBar.SetFieldValue<int>(dSFI.m_data, dSFI.m_scriptField.m_classField);
										if (dSFI.m_scriptField.m_fieldName == "m_health")
											sfi.m_data.m_health = dSFI.m_data;
										if (dSFI.m_scriptField.m_fieldName == "m_maxHealth")
											sfi.m_data.m_maxHealth = dSFI.m_data;
										if (dSFI.m_scriptField.m_fieldName == "healthBarUI")
											sfi.m_data.m_healthBarUI = dSFI.m_data;
									}

								}
								EndDisabled();
							}

							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<CharacterType>>())
							{
								GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
								//std::string pName = "##" + sfi.m_scriptField.m_fieldName;

								//std::cout << entity << "CT:: " << sfi.m_data << "\n";
								ImGui::TableNextRow();
								BeginDisabled(false);
								TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								TableNextColumn();
								if (ImGui::BeginCombo("##", CharacterTypeToString[sfi.m_data].c_str()))
								{
			
									for (size_t ic{ 0 }; ic < CharacterTypeToString.size(); ++ic)
									{
										bool is_selected{ false };
										if (sfi.m_data != static_cast<CharacterType>(ic))
										{

											//ImGui::Selectable(, is_selected);
											if (ImGui::Selectable(CharacterTypeToString[ic].c_str(), is_selected))
											{
												sfi.m_data = static_cast<CharacterType>(ic);
												s.SetFieldValue<CharacterType>(sfi.m_data, sfi.m_scriptField.m_classField);
											}
										}
										if (is_selected)
										{
											ImGui::SetItemDefaultFocus();
										}
									}
									// End the dropdown
									ImGui::EndCombo();
								}
									
									EndDisabled();


							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>())
							{
								GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
								ImGui::TableNextRow();
								BeginDisabled(false);
								TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								TableNextColumn(); 
								if (InputScriptList("##" + sfi.m_scriptField.m_fieldName, sfi.m_data, inputWidth)){ s.SetFieldValueArr<int>(sfi.m_data,sm->m_appDomain ,sfi.m_scriptField.m_classField);};
								EndDisabled();
							}
							else if (dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>())
							{
								GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
								ImGui::TableNextRow();
								BeginDisabled(false);
								TableNextColumn();
								ImGui::Text(sfi.m_scriptField.m_fieldName.c_str());
								TableNextColumn();
								if (InputScriptList("##" + sfi.m_scriptField.m_fieldName, sfi.m_data, inputWidth)) { s.SetFieldValueArr<unsigned>(sfi.m_data, sm->m_appDomain, sfi.m_scriptField.m_classField); };
								ImGui::EndDisabled();
							}

						}

						// Check if the mouse is over the second table and the right mouse button is clicked
						EndTable();
						ImGui::Separator();

					}
					ImGuiStyle& style = GetStyle();
					ImVec4 originalColor = style.Colors[ImGuiCol_Button];
					ImVec4 originalHColor = style.Colors[ImGuiCol_ButtonHovered];
					style.Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.28f, 0.66f, 1.0f);
					style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.48f, 0.86f, 1.0f);
					GE::AI::NodeEditor* ne = &GE::AI::NodeEditor::GetInstance();
					if (ImGui::Button("Add Script", ImVec2(GetWindowSize().x,0.0f))) {
						for (const std::string& sn : sm->m_allScriptNames)
						{
							auto it = std::find_if(allScripts->m_scriptList.begin(), allScripts->m_scriptList.end(), [sn](const ScriptInstance pair) { return pair.m_scriptName == sn; });
							auto it2 = std::find_if(ne->m_allScriptNames.begin(), ne->m_allScriptNames.end(), [sn](const std::string pair) { return pair == sn; });
							if (it == allScripts->m_scriptList.end() && it2 == ne->m_allScriptNames.end())
							{
								std::cout << sn << "\n";
								allScripts->m_scriptList.emplace_back(sn,entity);
								break;
							}
						}
					}
					style.Colors[ImGuiCol_Button] = originalColor;
					style.Colors[ImGuiCol_ButtonHovered] = originalHColor;
					ImGui::Separator();

				/*	bool displayPopup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsMouseClicked(1);
					if (displayPopup) {
						 << "Display delete\n";
					}
					else
					{
						 << "Nope\n";
					}*/
					for (const std::string& tds : toDeleteList)
					{
						auto it = std::find_if(allScripts->m_scriptList.begin(), allScripts->m_scriptList.end(), [tds](const ScriptInstance pair) { return pair.m_scriptName == tds; });
						allScripts->m_scriptList.erase(it);
					}
			
				}
			}
			else if (compType == rttr::type::get<Component::Draggable>())
			{
				if (CollapsingHeader("Draggable", ImGuiTreeNodeFlags_Leaf))
				{
					if (RemoveComponentPopup<Draggable>("Draggable", entity))
					{
						break;
					}
				}
			}
			else if (compType == rttr::type::get<Component::Text>())
			{
				if (ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Component::Text>("Text", entity))
					{
						break;
					}

					auto textObj = ecs.GetComponent<Component::Text>(entity);
					Separator();
					BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
					
					ImGui::TableNextColumn();
					ImGui::ColorEdit4("Color", textObj->m_clr.rgba);
					ImGui::DragFloat("Scale", &textObj->m_scale, .001f, 0.f, 5.f);
					ImGui::DragFloat("Width", &textObj->m_width, .001f, 0.f);
					if (ImGui::InputTextMultiline("Text", &textObj->m_text)) textObj->m_textInfo.flags.dirty = true;
					
					auto const& fontManager{ Graphics::GraphicsEngine::GetInstance().fontManager };
					auto const& fontLT{ fontManager.GetFontLT() }; //lookup table for fonts (string to ID)
					
					if (BeginCombo("Font", fontManager.GetFontName(textObj->m_fontID).c_str()))
					{
						for (auto const& it : fontLT)
						{
							if (Selectable(it.first.c_str()))
							{
								textObj->m_fontID = it.second;
							}
						}
						EndCombo();
					}
					if (BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_FONT"))
						{
							const char* droppedPath = static_cast<const char*>(payload->Data);
							GE::Component::Text* entityTextData = ecs.GetComponent<GE::Component::Text>(entity);
							entityTextData->m_fontID = Graphics::GraphicsEngine::GetInstance().fontManager.GetFontID(GE::GoopUtils::ExtractFilename(droppedPath));

						}
						EndDragDropTarget();
					}
					EndTable();
					Separator();
				}
			}
			else if (compType == rttr::type::get<Component::Audio>())
			{
				auto audio = ecs.GetComponent<GE::Component::Audio>(entity);
				if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Audio>("Audio", entity))
					{
						break;
					}

					Separator();
					InputList("Audio", audio->m_sounds, charSize);

					Separator();
				}
			}
			else if (compType == rttr::type::get<Component::GE_Button>())
			{
				auto button = ecs.GetComponent<GE::Component::GE_Button>(entity);

				if (CollapsingHeader("Button", ImGuiTreeNodeFlags_Leaf))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveButton");
					}
					if (BeginPopup("RemoveButton"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<GE_Button>(entity);
							EndPopup();
							break;
						}
						EndPopup();
					}

					rttr::type const type{ rttr::type::get<GE::Component::GE_Button::ButtonEventType>() };
					if (BeginCombo("Event", type.get_enumeration().value_to_name(button->m_eventType).to_string().c_str()))
					{
						for (GE_Button::ButtonEventType currType{}; currType != GE_Button::ButtonEventType::TOTAL_EVENTS;)
						{
							// get the string ...
							std::string str = type.get_enumeration().value_to_name(currType).to_string().c_str();

							if (Selectable(str.c_str(), currType == button->m_eventType))
							{
								button->m_eventType = currType; // set the current type if selected 
							}
							// and now iterate through
							currType = static_cast<GE_Button::ButtonEventType>(static_cast<int>(currType) + 1);
						}
						EndCombo();
					}
					InputText("Param", &button->m_param);
					ImGui::Separator();
				}
			}
			else if (compType == rttr::type::get<Component::Card>())
			{
				auto* card = ecs.GetComponent<GE::Component::Card>(entity);
				if (ImGui::CollapsingHeader("Card", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Component::Card>("Card", entity))
					{
						break;
					}


					ImGui::Separator();
					BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
					ImGui::TableNextColumn();

					// Value to store in card
					{

						ImGui::Text("Target Entity: ");
						ImGui::SameLine();
						int newVal{ static_cast<int>(card->tgtEntity) };
						ImGui::InputInt(("##CEntTgt" + std::to_string(entity)).c_str(), &newVal);
						card->tgtEntity = newVal;

						// check if entity has card holder
						{
							if (ecs.HasComponent<CardHolder>(card->tgtEntity))
							{
								ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "Target Has Holder Component");
							}
							else
							{
								ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Target Missing Holder Component");
							}
						}


						ImGui::Separator();
					}

					rttr::type const type{ rttr::type::get<GE::Component::Card::CardID>() };
					
					if (BeginCombo("Card", type.get_enumeration().value_to_name(card->cardID).to_string().c_str()))
					{
						ImGui::Text("This is being ported to C#");
						//for (Card::CardID currType{}; currType != Card::CardID::TOTAL_CARDS;)
						//{
						//	// get the string ...
						//	std::string str = type.get_enumeration().value_to_name(currType).to_string().c_str();

						//	if (Selectable(str.c_str(), currType == card->cardID))
						//	{
						//		card->cardID = currType; // set the current type if selected 
						//		
						//		if (ecs.HasComponent<Sprite>(entity))
						//		{
						//			auto* spriteComp = ecs.GetComponent<Sprite>(entity);
						//			auto const& textureManager = Graphics::GraphicsEngine::GetInstance().textureManager;
						//			GLuint texID{ textureManager.GetTextureID(CardSpriteNames[card->cardID]) };
						//			//Graphics::Texture const& newSprite = textureManager.GetTexture(texID);

						//			spriteComp->m_spriteData.texture = texID;
						//		}
						//	}
						//	// and now iterate through
						//	currType = static_cast<Card::CardID>(static_cast<int>(currType) + 1);
						//}
						EndCombo();
					}

					
					EndTable();
					Separator();
				}
			}
			else if (compType == rttr::type::get<Component::CardHolder>())
			{
				auto* cardHolder = ecs.GetComponent<GE::Component::CardHolder>(entity);

				if (ImGui::CollapsingHeader("Card Holder", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Component::CardHolder>("Card Holder", entity))
					{
						break;
					}

					ImGui::Separator();
					BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
					TableNextColumn();

					rttr::type const type{ rttr::type::get<GE::Component::Card::CardID>() };
					//ELEMENTS
					{
						// TITLE
						ImGui::TextColored(ImVec4{ 1.f, .733f, 0.f, 1.f },
							("ELEMENTS | Size: " + std::to_string(cardHolder->elements.size())).c_str());
						
						// + AND - BUTTONS
						if (Button("+"))
						{
							cardHolder->elements.emplace_back();
						}
						ImGui::SameLine();
						if (Button("-") && !cardHolder->elements.empty())
						{
							cardHolder->elements.pop_back();
						}

						int currElement{1}; // the current element to be displayed
						for (auto& element : cardHolder->elements)
						{
							ImGui::TextColored(ImVec4{ 1.f, 1.f, 0.f, 1.f }, 
								("Element: " + std::to_string(currElement)).c_str());
							{
								// THE DST ENTITY VALUE
								{
									ImGui::Text("Elem Entity: ");
									ImGui::SameLine();
									int newVal{ static_cast<int>(element.elemEntity) };
									if (InputInt(("##CDElmID" + std::to_string(currElement)).c_str(), &newVal))
									{
										element.elemEntity = newVal;
										
										// get other sprite component if it exists
										if (ecs.HasComponent<Sprite>(element.elemEntity))
										{
											auto* spriteComp = ecs.GetComponent<Sprite>(element.elemEntity);
											element.defaultSpriteID = spriteComp->m_spriteData.texture;
											if (ecs.HasComponent<CardHolderElem>(element.elemEntity))
											{
												auto* holderElem = ecs.GetComponent<CardHolderElem>(element.elemEntity);
												holderElem->elemIdx = currElement - 1; // since element started from 1, we subtract 1
												holderElem->holder = entity;
											}
											else
												GE::Debug::ErrorLogger::GetInstance().LogWarning((std::string(ecs.GetEntityName(element.elemEntity)) + 
													" is missing the CardHolderElem component! Add and reassign ID to Elem Entity in holder!").c_str());
										}
										else
										{
											element.defaultSpriteID = 0; // invalid
										}
									}
									if (element.elemEntity != ECS::INVALID_ID)
									{
										// get other sprite component if it exists
										if (ecs.HasComponent<Sprite>(element.elemEntity))
										{
											auto* spriteComp = ecs.GetComponent<Sprite>(element.elemEntity);
											element.defaultSpriteID = spriteComp->m_spriteData.texture;
										}
										else
										{
											element.defaultSpriteID = 0; // invalid
										}
									}
								}
								
								// THE SRC ENTITY VALUE
								{
									ImGui::Text("Card Entity: ");
									ImGui::SameLine();
									int newVal{ static_cast<int>(element.cardEntity) };
									if (InputInt(("##CDEntID" + std::to_string(currElement)).c_str(), &newVal))
									{
										element.cardEntity = newVal;
									}
									if (element.cardEntity != ECS::INVALID_ID)
									{
										// get other sprite component if it exists
										if (ecs.HasComponent<Sprite>(element.cardEntity))
										{
											auto* spriteComp = ecs.GetComponent<Sprite>(element.cardEntity);
											element.spriteID = spriteComp->m_spriteData.texture;
										}
										else
										{
											element.spriteID = 0; // invalid
										}
									}
								}
								// THE SPRITE ID (FOR REFERENCE)
								{
									BeginDisabled(true);
									// DEFAULT SPRITE
									ImGui::Text("Default Sprite: ");
									ImGui::SameLine();
									auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
									if (element.elemEntity != ECS::INVALID_ID && element.defaultSpriteID)
									{
										ImGui::TextColored(ImVec4{ 0.f, 1.f, 0.f, 1.f },
											(textureManager.GetTextureName(element.defaultSpriteID)
												+ std::string(" | ")).c_str());
										ImGui::SameLine();
										ImGui::TextColored(ImVec4{ 1.f, .7333f, 0.f, 1.f },
											ecs.GetEntityName(element.elemEntity).c_str());
									}
									else
									{
										ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, "No Sprite");
									}

									// USED SPRITE
									ImGui::Text("Used Sprite: ");
									ImGui::SameLine();
									if (element.cardEntity != ECS::INVALID_ID && element.spriteID)
									{
										ImGui::TextColored(ImVec4{ 0.f, 1.f, 0.f, 1.f },
											(textureManager.GetTextureName(element.spriteID)
												+ std::string(" | ")).c_str());
										ImGui::SameLine();
										ImGui::TextColored(ImVec4{ 1.f, .7333f, 0.f, 1.f },
											ecs.GetEntityName(element.cardEntity).c_str());
									}
									else
									{
										ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, "No Sprite");
									}
									ImGui::EndDisabled();
								}

								// THE USED FLAG
								{
									ImGui::Text("active");
									ImGui::SameLine();
									BeginDisabled(true);
									Checkbox(("##elemAct" + std::to_string(currElement)).c_str(), &element.used);
									ImGui::EndDisabled();
								}
							}
							++currElement;
						}

						//rttr::type const type{ rttr::type::get<GE::Component::GE_Button::ButtonEventType>() };
						rttr::type const DatatypeType{ rttr::type::get<GE::Component::CardHolder::DataType>() };
						if (BeginCombo("BRRR", DatatypeType.get_enumeration().value_to_name(cardHolder->dataType).to_string().c_str()))
						{
							for (unsigned int curr{}; curr != Component::CardHolder::DataType::NONE + 1; ++curr)
							{
								if (Selectable(DatatypeType.get_enumeration().value_to_name(curr).to_string().c_str(), 
									curr == cardHolder->dataType))
								{
									cardHolder->dataType = static_cast<Component::CardHolder::DataType>(curr);
								}
							}
							EndCombo();
						}
						{
							int value{static_cast<int>(cardHolder->targetScript)};
							ImGui::Text("Target Script Entity: ");
							ImGui::SameLine();
							ImGui::InputInt("##tgtSE", &value);
							cardHolder->targetScript = value;
						}
					}
					//if (BeginCombo("Card", type.get_enumeration().value_to_name(card->cardID).to_string().c_str()))
					//{
					//	for (Card::CardID currType{}; currType != Card::CardID::TOTAL_CARDS;)
					//	{
					//		// get the string ...
					//		std::string str = type.get_enumeration().value_to_name(currType).to_string().c_str();

					//		if (Selectable(str.c_str(), currType == card->cardID))
					//			card->cardID = currType; // set the current type if selected 
					//		// and now iterate through
					//		currType = static_cast<Card::CardID>(static_cast<int>(currType) + 1);
					//	}
					//	EndCombo();
					//}
					EndTable();
					Separator();
				}
			}
			else if (compType == rttr::type::get<Component::CardHolderElem>())
			{
				if (CollapsingHeader("Card Holder Element", ImGuiTreeNodeFlags_Leaf))
				{
					if (RemoveComponentPopup<Draggable>("Card Holder Element", entity))
					{
						break;
					}
					auto* holderElem = ecs.GetComponent<CardHolderElem>(entity);
					// FOR MEMBER: HOLDER ID
					{
						ImGui::Text("holder entity ID");
						ImGui::SameLine();
						int newHolderID{ static_cast<int>(holderElem->holder) };
						InputInt("##holderID", &newHolderID);
						if (newHolderID < -1)
							newHolderID = -1;
						holderElem->holder = newHolderID;
						if (holderElem->holder != ECS::INVALID_ID && ecs.HasComponent<CardHolder>(holderElem->holder))
							ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "Holder ID has holder component!");
						else
							ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Invalid holder/missing holder component!");
					}
					// FOR MEMBER: INDEX
					{
						ImGui::Text("holder entity index");
						ImGui::SameLine();
						ImGui::BeginDisabled();
						int newHolderID{ static_cast<int>(holderElem->elemIdx) };
						InputInt("##elemIdx", &newHolderID);
						ImGui::EndDisabled();
					}
				}
			}
			else if (compType == rttr::type::get<Component::Game>())
			{
				auto* game = ecs.GetComponent<GE::Component::Game>(entity);
				if (ImGui::CollapsingHeader("Game", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<GE::Component::Game>("Game", entity))
					{
						break;
					}

					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					ImGui::TableNextRow();

					InputEntity("Player Entity", game->m_player);
					ImGui::TableNextRow();
					InputEntity("Enemy Entity", game->m_enemy);
					ImGui::TableNextRow();
					InputEntity("Pause Menu", game->m_pauseMenu);
					TableNextRow();
					InputEntity("Player Hand", game->m_playerHand);
					TableNextRow();
					InputEntity("Player Queue", game->m_playerQueue);
					TableNextRow();
					InputEntity("Enemy Queue", game->m_enemyQueue);
					TableNextRow();
					TableNextColumn();
					ImGui::Text("Script");
					TableNextColumn();
					GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
					if (ImGui::BeginCombo("", game->m_gameSystemScript.m_scriptName.c_str()))
					{
						for (const std::string& sn : sm->m_allScriptNames)
						{
							if (game->m_gameSystemScript.m_scriptName != sn)
							{
								bool is_selected = (game->m_gameSystemScript.m_scriptName.c_str() == sn);
								if (ImGui::Selectable(sn.c_str()))
								{
									game->m_gameSystemScript = ScriptInstance(sn,entity);
									game->m_gameSystemScript.GetFields();
								}
								if (is_selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}
						}
						ImGui::EndCombo();
					}

					EndTable();
				}
				ImGui::Separator();
			}
			else if (compType == rttr::type::get<Component::Emitter>())
			{
				auto em = ecs.GetComponent<GE::Component::Emitter>(entity);
				if (ImGui::CollapsingHeader("Emitter", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Emitter>("Emitter", entity))
					{
						break;
					}
				}

				// Honestly no idea why -30 makes all 3 input fields match in size but sure
				float inputWidth = (contentSize - charSize - 30) / 3;

				Separator();
				PushID("Emitter");
				BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
				ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
				InputDouble3("Min Vel", em->m_minVel, inputWidth);
				TableNextRow();
				InputDouble3("Max Vel", em->m_maxVel, inputWidth);
				TableNextRow();
				InputDouble3("Gravity", em->m_gravity, inputWidth);
				TableNextRow();
				InputCheckBox("Play On Start", em->m_playOnStart);
				TableNextRow();
				InputCheckBox("Playing", em->m_playing);
				EndTable();
				InputFloat("Min Drag", &em->m_minDrag);
				InputFloat("Max Drag", &em->m_maxDrag);
				InputFloat("Min Lifetime", &em->m_minLifeTime);
				InputFloat("Max Lifetime", &em->m_maxLifeTime);
				InputInt("Emission Rate", &em->m_particlesPerMin);
				ImGui::Separator();
				ImGui::PopID();
			}
			else if (compType == rttr::type::get<Component::AnimEvents>())
			{
				//auto em = ecs.GetComponent<GE::Component::AnimEvents>(entity);
				if (ImGui::CollapsingHeader("AnimEvents", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<AnimEvents>("AnimEvents", entity))
					{
						break;
					}
				}
				//BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
				//ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);

				ImVec4 scriptNameColor{ 0.2f, 0.2f, 0.8f, 1.f };
				ImGui::TextColored(scriptNameColor, "Events Container");
				AnimEvents* comp = ecs.GetComponent<GE::Component::AnimEvents>(entity);
				
				if (Button("+"))
				{
					comp->m_eventList.emplace_back();
				}
				ImGui::SameLine();
				if (Button("-") && !comp->m_eventList.empty())
				{
					comp->m_eventList.pop_back();
				}
				size_t elemIdx{};
				for (auto& elem : comp->m_eventList)
				{
					// obj, string

					auto const& animManager{ Graphics::GraphicsEngine::GetInstance().animManager };
					auto const& textureLT{ animManager.GetAnimLT() };
					ImGui::Text("Animation: ");
					SameLine();
					if (BeginCombo(("##spriteEvtAnim" + std::to_string(elemIdx)).c_str(), animManager.GetAnimName(elem.first).c_str()))
					{
						for (auto const& it : textureLT)
						{
							if (Selectable(it.first.c_str()))
							{
								elem.first = it.second;
							}
						}
						EndCombo();
					}

					// Now settle for the current events preset in this element
					auto const& eventManager{ Events::AnimEventManager::GetInstance() };
					auto const& eventsTable{ eventManager.GetAnimEventsTable() };
					if (BeginCombo(("##animEvtList" + std::to_string(elemIdx)).c_str(), elem.second.c_str()))
					{
						for (auto const& it : eventsTable)
						{
							if (Selectable(it.first.c_str()))
							{
								elem.second = it.first;
							}
						}
						EndCombo();
					}
					++elemIdx;
				}
				//EndTable();
				ImGui::Separator();
			}
			else
			{
				GE::Debug::ErrorLogger::GetInstance().LogWarning("Trying to inspect a component that is not being handled: "
					+ compType.get_name().to_string());
			}
		}
	}

	ImGui::PopID();

	static bool addingComponent{ false };
	if (Button("Add Component", { GetContentRegionMax().x, 20 }))
	{
		addingComponent = true;
	}

	if (addingComponent)
	{
		if (BeginCombo("Components", ECS::componentTypes.front().get_name().to_string().c_str()))
		{
			for (unsigned i{}; i < ECS::componentTypes.size(); ++i)
			{		
				if (Selectable(ECS::componentTypes[i].get_name().to_string().c_str()))
				{
					std::stringstream ss;
					rttr::type const& compType{ ECS::componentTypes[i] };
					if (compType == rttr::type::get<Component::BoxCollider>())
					{
						if (!ecs.HasComponent<BoxCollider>(entity))
						{
							BoxCollider comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(BoxCollider).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Tween>())
					{
						if (!ecs.HasComponent<Tween>(entity))
						{
							Tween comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Tween).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Velocity>())
					{
						if (!ecs.HasComponent<Velocity>(entity))
						{
							Velocity comp;
							comp.m_mass = 1.0;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Velocity).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Sprite>())
					{
						if (!ecs.HasComponent<Sprite>(entity))
						{
							Sprite comp;
							auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
							auto const& textureLT{ textureManager.GetTextureLT() };
							auto textureIt = textureLT.begin(); // iterator for first texture (alphabetically)
							auto const& texture = textureManager.GetTexture(textureIt->second);
							comp.m_spriteName = textureIt->first;
							comp.m_spriteData.texture = textureIt->second;
							comp.m_spriteData.info.height = texture.height;
							comp.m_spriteData.info.width = texture.width;
							comp.m_spriteData.info.texDims = { 1.f, 1.f }; // default
							comp.m_spriteData.info.texCoords = {}; // bottom left
							
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Sprite).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::SpriteAnim>())
					{
						if (!ecs.HasComponent<SpriteAnim>(entity))
						{
							SpriteAnim comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(SpriteAnim).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Scripts>())
					{
						if (!ecs.HasComponent<Scripts>(entity))
						{
							Scripts comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Scripts).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Draggable>())
					{
						if (!ecs.HasComponent<Draggable>(entity))
						{
							Draggable comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Draggable).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Text>())
					{
						if (!ecs.HasComponent<Component::Text>(entity))
						{
							Component::Text comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Component::Text).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Audio>())
					{
						if (!ecs.HasComponent<Component::Audio>(entity))
						{
							Component::Audio comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Component::Audio).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::GE_Button>())
					{
						if (!ecs.HasComponent<GE::Component::GE_Button>(entity))
						{
							GE::Component::GE_Button comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(GE::Component::GE_Button).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Card>())
					{
						if (!ecs.HasComponent<GE::Component::Card>(entity))
						{
							GE::Component::Card comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(GE::Component::Card).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::CardHolder>())
					{
						if (!ecs.HasComponent<GE::Component::CardHolder>(entity))
						{
							GE::Component::CardHolder comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(GE::Component::CardHolder).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Game>())
					{
						if (!ecs.HasComponent<GE::Component::Game>(entity))
						{
							GE::Component::Game comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(GE::Component::Game).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::CardHolderElem>())
					{
						if (!ecs.HasComponent<CardHolderElem>(entity))
						{
							ecs.AddComponent(entity, CardHolderElem{});
						}
						else
						{
							ss << "Unable to add component " << typeid(CardHolderElem).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::EnemyAI>())
					{
						if (!ecs.HasComponent<EnemyAI>(entity))
						{
							ecs.AddComponent(entity, EnemyAI{});
						}
						else
						{
							ss << "Unable to add component " << typeid(EnemyAI).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::Emitter>())
					{
						if (!ecs.HasComponent<Emitter>(entity))
						{
							ecs.AddComponent(entity, Emitter{});
						}
						else
						{
							ss << "Unable to add component " << typeid(Emitter).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else if (compType == rttr::type::get<Component::AnimEvents>())
					{
						if (!ecs.HasComponent<AnimEvents>(entity))
						{
							ecs.AddComponent(entity, AnimEvents{});
						}
						else
						{
							ss << "Unable to add component " << typeid(AnimEvents).name() << ". Component already exist";
						}
						addingComponent = false;
						break;
					}
					else
					{
						ss << "Try to add an unhandled component";
						addingComponent = false;
					}
					addingComponent = false;

					if (!ss.str().empty())
					{
						GE::Debug::ErrorLogger::GetInstance().LogError(ss.str());
					}
				}
			}
			EndCombo();
		}
	}
}

namespace
{
	bool InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled)
	{
		bool valChanged{ false };

		BeginDisabled(disabled);
		ImGui::TableNextColumn();
		ImGui::Text(propertyName.c_str());
		propertyName = "##" + propertyName;
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(fieldWidth);
		if (InputDouble((propertyName + "X").c_str(), &property.x, 0, 0, "%.5f")) { valChanged = true; };
		SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Y").c_str(), &property.y, 0, 0, "%.5f")) { valChanged = true; };
		SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Z").c_str(), &property.z, 0, 0, "%.5f")) { valChanged = true; };
		ImGui::EndDisabled();

		return valChanged;
	}

	void InputDouble1(std::string propertyName, double& property, bool disabled)
	{
		BeginDisabled(disabled);
		ImGui::TableNextColumn();
		ImGui::Text(propertyName.c_str());
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(GetWindowSize().x);
		InputDouble(("##" + propertyName).c_str(), &property, 0, 0, "%.2f");
		ImGui::EndDisabled();
	}
	
	bool InputCheckBox(std::string propertyName, bool& property, bool disabled)
	{
		PushID(propertyName.c_str());
		bool valChanged{ false };
		BeginDisabled(disabled);
		ImGui::TableNextColumn();
		ImGui::Text(propertyName.c_str());
		ImGui::TableNextColumn();
		valChanged = Checkbox(("##" + propertyName).c_str(), &property);
		ImGui::EndDisabled();
		ImGui::PopID();
		return valChanged;
	}

	bool InputEntity(std::string propertyName, GE::ECS::Entity& entity, bool disabled)
	{
		int tempEntity = static_cast<int>(entity);
		BeginDisabled(disabled);
		ImGui::TableNextColumn();
		ImGui::Text(propertyName.c_str());
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(GetWindowSize().x);
		if (InputInt(("##" + propertyName).c_str(), &tempEntity, 0))
		{
			entity = static_cast<GE::ECS::Entity>(tempEntity);
			ImGui::EndDisabled();
			return true;
		}
		ImGui::EndDisabled();
		return false;
	}

	template <>
	void InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			ImGui::BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			ImGui::TableNextRow();
			int i{};
			for (auto& force : list)
			{
				PushID((std::to_string(i++)).c_str());
				InputDouble3("Force", force.m_magnitude, fieldWidth, disabled);
				InputDouble1("Lifetime", force.m_lifetime);
				InputDouble1("Age", force.m_age);
				InputCheckBox("IsActive", force.m_isActive);
				ImGui::PopID();
				ImGui::Separator();
			}
			ImGui::EndTable();

			ImGui::Separator();
			ImGui::Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.push_back(LinearForce());
			}

			ImGui::TreePop();
		}
		Indent();
	}

	template <>
	void InputList(std::string propertyName, std::deque<GE::Math::dVec3>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			ImGui::BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			int i{};
			for (auto& item : list)
			{
				PushID((std::to_string(i++)).c_str());
				InputDouble3(propertyName + " " + std::to_string(i++), item, fieldWidth, disabled);
				ImGui::TableNextRow();
				ImGui::PopID();
			}
			ImGui::EndTable();

			ImGui::Separator();
			ImGui::Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.push_back({ 0, 0, 0 });
			}

			ImGui::TreePop();
		}
		Indent();
	}

	template<>
	void InputList(std::string propertyName, std::map<std::string, std::vector<GE::Component::Tween::Action>>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;
		auto removeTweenIt{ list.begin() };
		for (auto& [animationName, action] : list)
		{
			std::string temp{animationName};
			if (TreeNodeEx(("Animation: " + animationName).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				SameLine();
				if (Button("Remove Animation"))
				{
					TreePop();
					break;
				}
				else
				{
					++removeTweenIt;
				}

				Separator();
				int i{};
				int removeIndex{};
				bool shouldRemove{ false };
				for (auto& [target, scale, rot, duration] : action)
				{
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH);
					TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
					PushID((std::to_string(i)).c_str());
					// Formatting
					if (i != 0)
					{
						TableNextRow();
						TableNextRow();
					}
					InputDouble3("Translate " + std::to_string(i), target, fieldWidth, disabled);
					InputDouble3("Scale " + std::to_string(i), scale, fieldWidth, disabled);
					InputDouble3("Rotate " + std::to_string(i), rot, fieldWidth, disabled);
					InputDouble1("Duration", duration);
					PopID();
					EndTable();
					if (Button("Delete keyframe", { GetContentRegionMax().x, 20 }))
					{
						removeIndex = i;
						shouldRemove = true;
						break;
					}
					++i;
				}
				if (shouldRemove)
				{
					action.erase(action.begin() + removeIndex);
				}

				Separator();
				Unindent();
				// 20 magic number cuz the button looks good
				if (Button("Add keyframe", { GetContentRegionMax().x, 20 }))
				{
					action.emplace_back(vec3{ 0, 0, 0 }, vec3{ 1, 1, 1 }, vec3{ 0, 0, 0 }, 1);
				}
				Indent();

				TreePop();
			}
		}

		if (removeTweenIt != list.end())
		{
			list.erase(removeTweenIt);
		}
	}

	template <>
	void InputList(std::string propertyName, std::vector<GE::ECS::Entity>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			ImGui::BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (unsigned int i{}; i < list.size(); ++i)
			{
				GE::ECS::Entity& entity{ i };
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				ImGui::TableNextColumn();
				InputEntity("Entity", entity);
				ImGui::TableNextRow();
				ImGui::PopID();
			}
			ImGui::EndTable();
			ImGui::Separator();
			ImGui::TreePop();
		}
	}

	template <>
	void InputList(std::string propertyName, std::vector<int>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			ImGui::BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (int i{}; i < list.size(); ++i)
			{
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				ImGui::TableNextColumn();
				InputInt(("##" + (propertyName + std::to_string(i))).c_str(), &list[i], 0);
				ImGui::TableNextRow();
				ImGui::PopID();
			}
			ImGui::EndTable();
			ImGui::Separator();
			ImGui::TreePop();
		}
	}

	bool InputScriptList(std::string propertyName, std::vector<int>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;
		bool changed{ false };
		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (int i{}; i < list.size(); ++i)
			{
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				ImGui::TableNextColumn();
				if (InputInt(("##" + (propertyName + std::to_string(i))).c_str(), &list[i], 0)) { changed = true; }
				TableNextRow();
				ImGui::PopID();
			}
			EndTable();
			ImGui::Separator();
			ImGui::TreePop();
		}
		return changed;
	}

	bool InputScriptList(std::string propertyName, std::vector<unsigned>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;
		bool changed{ false };
		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (int i{}; i < list.size(); ++i)
			{
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				ImGui::TableNextColumn();
				int rep = static_cast<int>(list[i]);
				if (InputInt(("##" + (propertyName + std::to_string(i))).c_str(), &rep, 0)) { 
					changed = true; 
					list[i] = static_cast<unsigned>(rep);
				}
				ImGui::TableNextRow();
				ImGui::PopID();
			}
			EndTable();
			ImGui::Separator();
			ImGui::TreePop();
		}
		return changed;
	}

	bool InputCardID(std::string propertyName, std::vector<Card::CardID>& list, float fieldWidth, bool disabled)
	{

		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;
		bool changed{ false };

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (int i{}; i < list.size(); ++i)
			{
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				ImGui::TableNextColumn();
				int rep = static_cast<int>(list[i]);
				if (InputInt(("##" + (propertyName + std::to_string(i))).c_str(), &rep, 0)) {
					changed = true;
					list[i] = static_cast<Card::CardID>(rep);
				}
				ImGui::TableNextRow();
				ImGui::PopID();
			}
			EndTable();
			ImGui::Separator();
			ImGui::TreePop();
		}
		return changed;
	}

	template <>
	void InputList(std::string propertyName, std::vector<GE::Component::Audio::Sound>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<std::vector<GE::Component::Audio::Sound>::iterator> removeIt;
			int i{};
			for (auto& sound : list)
			{
				PushID((std::to_string(i)).c_str());
				ImGui::Separator();
				BeginTable("Whoooo", 2, ImGuiTableFlags_BordersInnerV);
				TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);

				TableNextColumn();
				ImGui::Text("Sound Name");
				ImGui::TableNextColumn();
				InputText("##", &sound.m_sound);
				if (BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = AcceptDragDropPayload("ASSET_BROWSER_AUDIO"))
					{
						sound.m_sound = static_cast<const char*>(payload->Data);
					}
					EndDragDropTarget();
				}

				ImGui::TableNextRow();
				InputCheckBox("Loop", sound.m_loop);
				ImGui::TableNextRow();
				InputCheckBox("Play on Start", sound.m_playOnStart);
				ImGui::TableNextRow();
				InputCheckBox("Paused", sound.m_paused);
				TableNextRow();
				TableNextColumn();
				ImGui::Text("Volume");
				TableNextColumn();
				if (DragFloat(("##" + sound.m_sound + "volume").c_str(), &sound.m_volume, 0.001f, 0.f, 1.f))
				{
					GE::fMOD::FmodSystem::GetInstance().SetVolume(sound.m_sound, sound.m_volume);
				}
				EndTable();

				if (BeginCombo("Channel", GE::fMOD::FmodSystem::m_channelToString.at(sound.m_channel).c_str()))
				{
					for (auto const& [channel, audioName] : GE::fMOD::FmodSystem::m_channelToString)
					{
						if (Selectable(audioName.c_str()))
						{
							sound.m_channel = channel;
						}
					}
					EndCombo();
				}

				if (Button("Remove"))
				{
					removeIt.emplace_back(list.begin() + i);
				}

				ImGui::PopID();
				++i;
			}

			for (auto it : removeIt)
			{				
				list.erase(it);
			}

			ImGui::Separator();

			ImGui::Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.emplace_back(GE::Component::Audio::Sound{});
			}

			ImGui::TreePop();
		}
		Indent();
	}

	template <typename T>
	bool RemoveComponentPopup(std::string name, GE::ECS::Entity entity)
	{
		std::string labelStr = ("Remove" + name);
		const char* labelCStr = labelStr.c_str();
		if (IsItemClicked(ImGuiMouseButton_Right))
		{
			OpenPopup(labelCStr);
		}
		if (BeginPopup(labelCStr))
		{
			if (Selectable("Remove Component"))
			{
				static GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
				ecs.RemoveComponent<T>(entity);
				EndPopup();

				return true;
			}
			EndPopup();
		}
		return false;
	}
}
#endif
