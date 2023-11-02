/*!*********************************************************************
\file   Inspector.cpp
\author w.chinkitbryam\@digipen.edu
\date   23 October 2023
\brief
	Wrapper class to create an EditorGUI for the tool bar

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ImGuiUI.h"
#include "Inspector.h"
#include <ImGui/imgui.h>
#include <Component/BoxCollider.h>
#include <Component/Model.h>
#include <Component/ScriptHandler.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/Transform.h>
#include <Component/Tween.h>
#include <Component/Velocity.h>
#include <Component/Draggable.h>
#include <Component/Text.h>
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>

// Disable empty control statement warning
#pragma warning(disable : 4390)

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
	************************************************************************/
	void InputCheckBox(std::string propertyName, bool& property, bool disabled = false);

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
		Wrapper for to create specialized inspector list of vector of 
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
		Wrapper for to create specialized inspector list of deque of
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
					
					bool valChanged{ false };

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
					if (valChanged) 
						GE::Systems::PostRootTransformSystem::Propergate(ecs, entity, trans->m_parentWorldTransform);
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
			{
				auto col = ecs.GetComponent<BoxCollider>(entity);
				if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveCollider");
					}
					if (BeginPopup("RemoveCollider"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<BoxCollider>(entity);
						}
						EndPopup();
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
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveVelocity");
					}
					if (BeginPopup("RemoveVelocity"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Velocity>(entity);
						}
						EndPopup();
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
				//auto sprite = ecs.GetComponent<Sprite>(entity);
				if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveSprite");
					}
					if (BeginPopup("RemoveSprite"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Sprite>(entity);
						}
						EndPopup();
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
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
			{
				//auto anim = ecs.GetComponent<SpriteAnim>(entity);
				if (ImGui::CollapsingHeader("Sprite Animation", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveSpriteAnimation");
					}
					if (BeginPopup("RemoveSpriteAnimation"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<SpriteAnim>(entity);
						}
						EndPopup();
					}
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::MODEL:
			{
				//auto model = ecs.GetComponent<Model>(entity);
				if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveModel");
					}
					if (BeginPopup("RemoveModel"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Model>(entity);
						}
						EndPopup();
					}
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
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveTween");
					}
					if (BeginPopup("RemoveTween"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Tween>(entity);
						}
						EndPopup();
					}

					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Time Elapsed", tween->m_timeElapsed);
					TableNextRow();
					InputDouble1("Time Between", tween->m_timePerTween);
					TableNextRow();
					InputDouble3("Last End Point", tween->m_originalPos, inputWidth);
					EndTable();
					InputList("Tween", tween->m_tweens, inputWidth);
					Separator();
				}

				break;
			}
			case GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
			{
				//auto scripts = ecs.GetComponent<ScriptHandler>(entity);
				if (ImGui::CollapsingHeader("Script Handler", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveScriptHandler");
					}
					if (BeginPopup("RemoveScriptHandler"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<ScriptHandler>(entity);
						}
						EndPopup();
					}
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
			{
				if (CollapsingHeader("Draggable", ImGuiTreeNodeFlags_Leaf))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("Remo");
					}
					if (BeginPopup("RemoveComponent"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Draggable>(entity);
						}
						EndPopup();
					}
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::TEXT:
			{
				//float inputWidth = (contentSize - charSize) / 3;
				if (ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (IsItemClicked(ImGuiMouseButton_Right))
					{
						OpenPopup("RemoveComponent");
					}
					if (BeginPopup("RemoveComponent"))
					{
						if (Selectable("Remove Component"))
						{
							ecs.RemoveComponent<Component::Text>(entity);
						}
						EndPopup();
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
					EndTable();
					Separator();
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
					case GE::ECS::COMPONENT_TYPES::MODEL:
					{
						if (!ecs.HasComponent<Model>(entity))
						{
							Model comp{};
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(Model).name() << ". Component already exist";
						}
						break;
					}
					case GE::ECS::COMPONENT_TYPES::TWEEN:
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
						break;
					}
					case GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
					{
						if (!ecs.HasComponent<ScriptHandler>(entity))
						{
							ScriptHandler comp;
							ecs.AddComponent(entity, comp);
						}
						else
						{
							ss << "Unable to add component " << typeid(ScriptHandler).name() << ". Component already exist";
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
	
	void InputCheckBox(std::string propertyName, bool& property, bool disabled)
	{
		BeginDisabled(disabled);
		TableNextColumn();
		ImGui::Text(propertyName.c_str());
		TableNextColumn();
		Checkbox(("##" + propertyName).c_str(), &property);
		EndDisabled();
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
}
