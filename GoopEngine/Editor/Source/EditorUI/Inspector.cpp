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
#ifndef NO_IMGUI
#include "ImGuiUI.h"
#include "Inspector.h"
#include <ImGui/imgui.h>
#include <Component/Components.h>
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Commands/CommandManager.h>
#include <Graphics/GraphicsEngine.h>
#include <EditorUI/GizmoEditor.h>
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

	\param[in] property
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

	\param[in] property
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

	\param[in] fieldWidth
		Width of input field

	\param[in] disabled
		Draw disabled

	\return
		If value has changed
	************************************************************************/
	bool InputCheckBox(std::string propertyName, bool& property, bool disabled = false);

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
	void InputList(std::string propertyName, std::deque<GE::Component::Tween::Action>& list, float fieldWidth, bool disabled);

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

	PushID(entity);
	bool isActive{ecs.GetIsActiveEntity(entity)};
	if (Checkbox("##IsActive", &isActive))
	{
		ecs.SetIsActiveEntity(entity, isActive);
	}
	SameLine();
	std::string name = ecs.GetEntityName(entity);
	if (InputText("##Name", &name))
	{
		ecs.SetEntityName(entity, name);
	}
	ImGui::Text(("Entity ID: " + std::to_string(entity)).c_str());
	//ImGui::ImageButton(reinterpret_cast<ImTextureID>(assetManager.GetID(file.path().string())), { newW, newH }, { 0, 1 }, { 1, 0 });

	/*if (ImageButton(reinterpret_cast<ImTextureID>(ecs.GetComponent<GE::Component::Sprite>(entity)->m_spriteData.texture), { 0, 0 }, { 0, 1 }, { 1, 0 }))
	{

	}*/
	GizmoEditor::SetVisible(false);
	for (int i{}; i < GE::ECS::MAX_COMPONENTS; ++i)
	{
		if (sig[i])		
		{
			float contentSize = GetWindowSize().x;
			// 15 characters for property name
			float charSize = CalcTextSize("012345678901234").x;
			switch (static_cast<GE::ECS::COMPONENT_TYPES>(i))
			{
			case GE::ECS::COMPONENT_TYPES::TRANSFORM:
			{
				auto trans = ecs.GetComponent<Transform>(entity);
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					// Honestly no idea why -30 makes all 3 input fields match in size but sure
					float inputWidth = (contentSize - charSize - 30) / 3;
					GE::CMD::PRS oldPRS{ trans->m_pos, trans->m_rot, trans->m_scale };
					bool valChanged{ false };

					Separator();
					// SET GIZMO RADIO BUTTONS
					{
						// SET MODE BASED ON KEYBINDS: W,E,R (trans, rot, scale)
						{
							if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_W))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::TRANSLATE);
							if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::ROTATE);
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
								GizmoEditor::GetCurrOperation() == ImGuizmo::OPERATION::ROTATE))
								GizmoEditor::SetOperation(ImGuizmo::OPERATION::ROTATE);
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


					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					TableNextRow();
					if (InputDouble3("Position", trans->m_pos, inputWidth)) { valChanged = true; };
					TableNextRow();
					if (InputDouble3("Scale", trans->m_scale, inputWidth)) { valChanged = true; };
					TableNextRow();
					if (InputDouble3("Rotation", trans->m_rot, inputWidth)) { valChanged = true; };

					TableNextRow();
					InputDouble3("World Position", trans->m_worldPos, inputWidth, true);
					TableNextRow();
					InputDouble3("World Scale", trans->m_worldScale, inputWidth, true);
					TableNextRow();
					InputDouble3("World Rotation", trans->m_worldRot, inputWidth, true);

					EndTable();
					Separator();
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
				break;
			}
			case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
			{
				auto col = ecs.GetComponent<BoxCollider>(entity);
				if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<BoxCollider>("Collider", entity))
					{
						break;
					}

					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Width", col->m_width);
					TableNextRow();
					InputDouble1("Height", col->m_height);
					TableNextRow();
#ifndef NO_IMGUI
					InputCheckBox("Show Collider", col->m_render);
#endif
					EndTable();
					Separator();
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::VELOCITY:
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
					TableNextRow();
					InputDouble1("Mass", vel->m_mass);
					TableNextRow();
					InputDouble3("Gravity", vel->m_gravity, inputWidth);		
					TableNextRow();
					InputDouble1("Drag", vel->m_dragForce.m_magnitude);
					TableNextRow();
					InputCheckBox("Drag Active", vel->m_dragForce.m_isActive);
					EndTable();
					InputList("Forces", vel->m_forces, inputWidth);
					Separator();
					PopID();
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE:
			{
				auto sprite = ecs.GetComponent<Sprite>(entity);
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
								sprite->m_spriteData.texture = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(droppedPath));

							}
							EndDragDropTarget();
						}
					}
					//ImGui::Columns(1);

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
								spriteObj->m_spriteData.texture = texture.textureHandle;
								spriteObj->m_spriteData.info.height = texture.height;
								spriteObj->m_spriteData.info.width = texture.width;
								spriteObj->m_spriteData.info.texDims = { 1.f, 1.f }; // default
								spriteObj->m_spriteData.info.texCoords = {}; // bottom left
							}
						}
						EndCombo();
					}
					EndTable();
					Separator();
#pragma endregion
#pragma region SPRITE_DEBUG_INFO 
					// texcoordinates and info you can't edit
					BeginDisabled();
					int imageDims[2]{ static_cast<int>(spriteObj->m_spriteData.info.width), 
														static_cast<int>(spriteObj->m_spriteData.info.height) };
					ImGui::InputInt("Image Width", &imageDims[0]);
					ImGui::InputInt("Image Width", &imageDims[1]);
					ImGui::InputFloat2("Tex Coords", &spriteObj->m_spriteData.info.texCoords.r);
					EndDisabled();
#pragma endregion
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
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
					if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj->m_animID).c_str()))
					{
						for (auto const& it : textureLT)
						{
							if (Selectable(it.first.c_str()))
							{
								auto const& anim = animManager.GetAnim(it.second);
								auto spriteObj = ecs.GetComponent<Component::Sprite>(entity);
								spriteAnimObj->m_animID = it.second;
								spriteAnimObj->m_currFrame = 0;
								spriteAnimObj->m_currTime	 = 0;

								// setting correct attributes for sprite
								auto const& textureManager{ Graphics::GraphicsEngine::GetInstance().textureManager };
								textureManager.GetTextureName(anim.texture);

								// Set sprite name and the texture handle
								spriteObj->m_spriteName = textureManager.GetTextureName(anim.texture);
								spriteObj->m_spriteData.texture = anim.texture;

								spriteObj->m_spriteData.info = anim.frames[0]; // and set actual sprite info
							}
						}
						EndCombo();
					}
					EndTable();
					Separator();
#pragma endregion
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::TWEEN:
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
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Time Elapsed", tween->m_timeElapsed);
					TableNextRow();
					InputDouble3("Last End Point", tween->m_originalPos, inputWidth);
					TableNextRow();
					InputCheckBox("Paused", tween->m_paused);
					EndTable();
					InputList("Tween", tween->m_tweens, inputWidth);
					Separator();
				}

				break;
			}
			case GE::ECS::COMPONENT_TYPES::SCRIPTS:
			{
				//auto scripts = ecs.GetComponent<Scripts>(entity);
				if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
				{
					//float inputWidth = (contentSize - charSize - 30) / 3;
					//GE::Component::Scripts* allScripts = ecs.GetComponent<Scripts>(entity);
					if (RemoveComponentPopup<Scripts>("Script", entity))
					{
						break;
					}
					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					//GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
					//for (std::pair<std::string, ScriptInstance> s : allScripts->m_scriptMap)
					//{
					//	/*TableNextRow();
					//	BeginDisabled(false);
					//	TableNextColumn();
					//	ImGui::Text("Scripts");
					//	TableNextColumn();
					//	SetNextItemWidth(GetWindowSize().x);
					//	if (ImGui::BeginCombo("Scripts", s.first.c_str(), ImGuiComboFlags_NoArrowButton))
					//	{
					//		for (const std::string& sn : sm->m_allScriptNames)
					//		{
					//			bool is_selected = (s.first.c_str() == sn);
					//			if (ImGui::Selectable(sn.c_str(), is_selected))
					//			{
					//				if(sn != s.first.c_str()){
					//					std::cout << "selected: " << sn << "\n";
					//				}
					//				
					//			}
					//			if (is_selected)
					//			{
					//				ImGui::SetItemDefaultFocus();
					//			}
					//		}
					//		ImGui::EndCombo();
					//	}
					//	EndDisabled();*/



					//	//const auto& fields = s.second.m_scriptClassInfo.m_ScriptFieldMap;
					//	//for (const auto& [fieldName, field] : fields)
					//	//{
					//	//	if (field.m_fieldType == ScriptFieldType::Float)
					//	//	{
					//	//	}
					//	//	else if (field.m_fieldType == ScriptFieldType::Int)
					//	//	{
					//	//		TableNextRow();
					//	//		//BeginDisabled(false);
					//	//		//TableNextColumn();
					//	//		//ImGui::Text(fieldName.c_str());
					//	//		//TableNextColumn();
					//	//		//SetNextItemWidth(GetWindowSize().x);
					//	//		//if (ImGui::InputInt(("##" + fieldName).c_str(), &data, 0, 0, 0)) { s.second.SetFieldValue(fieldName, data); }
					//	//		//EndDisabled();
					//	//	}
					//	//	else if (field.m_fieldType == ScriptFieldType::Double)
					//	//	{
					//	//	}
					//	//	else if (field.m_fieldType == ScriptFieldType::DVec3)
					//	//	{
					//	//		TableNextRow();
					//	//	}
					//	//	else if (field.m_fieldType == ScriptFieldType::IntArr)
					//	//	{
					//	//		TableNextRow();
					//	//	}
					//	//}
					//}



					EndTable();
					Separator();

				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
			{
				if (CollapsingHeader("Draggable", ImGuiTreeNodeFlags_Leaf))
				{
					if (RemoveComponentPopup<Draggable>("Draggable", entity))
					{
						break;
					}
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::TEXT:
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
					
					TableNextColumn();
					ImGui::ColorEdit4("Color", textObj->m_clr.rgba);
					ImGui::DragFloat("Scale", &textObj->m_scale, .001f, 0.f, 5.f);
					ImGui::InputTextMultiline("Text", &textObj->m_text);
					
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
				break;
			}
			case GE::ECS::COMPONENT_TYPES::AUDIO:
			{
				auto audio = ecs.GetComponent<GE::Component::Audio>(entity);
				if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Audio>("Audio", entity))
					{
						break;
					}

					Separator();
					InputText("Sound File", &audio->m_name);
					if (BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = AcceptDragDropPayload("ASSET_BROWSER_AUDIO"))
						{
							audio->m_name = static_cast<const char*>(payload->Data);
						}
						EndDragDropTarget();
					}

					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					TableNextRow();
					InputCheckBox("Loop", audio->m_loop);
					TableNextRow();
					InputCheckBox("Play on Start", audio->m_playOnStart);
					TableNextRow();
					InputCheckBox("Paused", audio->m_paused);
					EndTable();

					if (BeginCombo("Channel", GE::fMOD::FmodSystem::m_channelToString.at(audio->channel).c_str()))
					{
						for (auto const& [channel, audioName] : GE::fMOD::FmodSystem::m_channelToString)
						{
							if (Selectable(audioName.c_str()))
							{
								audio->channel = channel;
							}
						}
						EndCombo();
					}

					Separator();
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::ANCHOR:
			{
				auto anchor = ecs.GetComponent<Anchor>(entity);

				if (ImGui::CollapsingHeader("Anchor", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (RemoveComponentPopup<Anchor>("Anchor", entity))
					{
						break;
					}

					Separator();

					if (BeginCombo("Anchor Type", Anchor::toString(anchor->m_type).c_str()))
					{
						for (int j{}; j < Anchor::TOTAL_TYPES; ++j)
						{
							std::string propName = Anchor::toString(static_cast<Anchor::ANCHOR_TYPE>(j));

							if (Selectable(propName.c_str()))
							{
								anchor->m_type = Anchor::toType(propName);
							}
						}
						EndCombo();
					}

					if (anchor->m_type == Anchor::IS_ANCHOR)
					{
						InputList("Anchor", anchor->m_anchored, charSize);
					}
					Separator();
        }
        break;
      }
			case GE::ECS::COMPONENT_TYPES::GE_BUTTON:
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
					Separator();
					InputText("Next Scene", &button->m_nextScene);

				}
				break;
			}
			default:
				GE::Debug::ErrorLogger::GetInstance().LogWarning("Trying to inspect a component that is not being handled");
				break;
			}
		}
	}

	PopID();

	static bool addingComponent{ false };
	if (Button("Add Component", { GetContentRegionMax().x, 20 }))
	{
		addingComponent = true;
	}

	if (addingComponent)
	{
		if (BeginCombo("Components", GE::ECS::componentsToString.at(static_cast<GE::ECS::COMPONENT_TYPES>(0)).c_str()))
		{
			for (int i{}; i < GE::ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
			{				
				if (Selectable(GE::ECS::componentsToString.at(static_cast<GE::ECS::COMPONENT_TYPES>(i)).c_str()))
				{
					std::stringstream ss;
					switch (i)
					{
					case GE::ECS::COMPONENT_TYPES::TRANSFORM:
					{
						if (!ecs.HasComponent<Transform>(entity))
						{
							Transform comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Transform).name() << ". Component already exist";
						}
						break;
					}
					case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::VELOCITY:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::SPRITE:
					{
						if (!ecs.HasComponent<Sprite>(entity))
						{
							Sprite comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Sprite).name() << ". Component already exist";
						}
						break;
					}
					case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::SCRIPTS:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::TEXT:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::AUDIO:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::ANCHOR:
					{
						if (!ecs.HasComponent<Component::Anchor>(entity))
						{
							Component::Anchor comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Component::Anchor).name() << ". Component already exist";
						}
						break;
					}
					case GE::ECS::COMPONENT_TYPES::GE_BUTTON:
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
						break;
					}
					default:
						break;
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
		TableNextColumn();
		ImGui::Text(propertyName.c_str());
		propertyName = "##" + propertyName;
		TableNextColumn();
		SetNextItemWidth(fieldWidth);
		if (InputDouble((propertyName + "X").c_str(), &property.x, 0, 0, "%.5f")) { valChanged = true; };
		SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Y").c_str(), &property.y, 0, 0, "%.5f")) { valChanged = true; };
		SameLine(0, 3); SetNextItemWidth(fieldWidth); if (InputDouble((propertyName + "Z").c_str(), &property.z, 0, 0, "%.5f")) { valChanged = true; };
		EndDisabled();

		return valChanged;
	}

	void InputDouble1(std::string propertyName, double& property, bool disabled)
	{
		BeginDisabled(disabled);
		TableNextColumn();
		ImGui::Text(propertyName.c_str());
		TableNextColumn();
		SetNextItemWidth(GetWindowSize().x);
		InputDouble(("##" + propertyName).c_str(), &property, 0, 0, "%.2f");
		EndDisabled();
	}
	
	bool InputCheckBox(std::string propertyName, bool& property, bool disabled)
	{
		bool valChanged{ false };
		BeginDisabled(disabled);
		TableNextColumn();
		ImGui::Text(propertyName.c_str());
		TableNextColumn();
		valChanged = Checkbox(("##" + propertyName).c_str(), &property);
		EndDisabled();
		return valChanged;
	}

	template <>
	void InputList(std::string propertyName, std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			TableNextRow();
			int i{};
			for (auto& force : list)
			{
				PushID((std::to_string(i++)).c_str());
				InputDouble3("Force", force.m_magnitude, fieldWidth, disabled);
				InputDouble1("Lifetime", force.m_lifetime);
				InputDouble1("Age", force.m_age);
				InputCheckBox("IsActive", force.m_isActive);
				PopID();
				Separator();
			}
			EndTable();

			Separator();
			Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.push_back(LinearForce());
			}

			TreePop();
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
			Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			int i{};
			for (auto& item : list)
			{
				PushID((std::to_string(i++)).c_str());
				InputDouble3(propertyName + " " + std::to_string(i++), item, fieldWidth, disabled);
				TableNextRow();
				PopID();
			}
			EndTable();

			Separator();
			Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.push_back({ 0, 0, 0 });
			}

			TreePop();
		}
		Indent();
	}

	template<>
	void InputList(std::string propertyName, std::deque<GE::Component::Tween::Action>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			int i{};
			for (auto& [target, duration] : list)
			{
				PushID((std::to_string(i)).c_str());
				InputDouble3(propertyName + " " + std::to_string(i++), target, fieldWidth, disabled);
				TableNextRow();
				TableNextColumn();
				TableNextColumn();
				InputDouble1("Duration", duration);
				TableNextRow();
				PopID();
			}
			EndTable();

			Separator();
			Unindent();
			// 20 magic number cuz the button looks good
			if (Button(("Add " + propertyName).c_str(), { GetContentRegionMax().x, 20 }))
			{
				list.emplace_back(vec3{ 0, 0, 0 }, 0);
			}

			TreePop();
		}
		Indent();
	}

	template <>
	void InputList(std::string propertyName, std::vector<GE::ECS::Entity>& list, float fieldWidth, bool disabled)
	{
		// 12 characters for property name
		float charSize = CalcTextSize("012345678901").x;

		if (TreeNodeEx((propertyName + "s").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			Separator();
			BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, charSize);
			for (unsigned int i{}; i < list.size(); ++i)
			{
				GE::ECS::Entity& entity{ i };
				PushID((std::to_string(i)).c_str());
				ImGui::Text(propertyName.c_str());
				TableNextColumn();
				int tempEntity = static_cast<int>(entity);
				InputInt("##", &tempEntity);
				list[i] = static_cast<GE::ECS::Entity>(tempEntity);
				if (BeginDragDropTarget())
				{
					const ImGuiPayload* pl = AcceptDragDropPayload("ENTITY");
					list[i] = *static_cast<GE::ECS::Entity*>(pl->Data);
					EndDragDropTarget();
				}
				TableNextRow();
				PopID();
			}
			EndTable();
			Separator();
			TreePop();
		}
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