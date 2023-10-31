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
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <Systems/RootTransform/LocalToWorldTransform.h>
#include <Systems/RootTransform/WorldToLocalTransform.h>

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

	if (entity == GE::ECS::INVALID_ID)
		return;

	GE::ECS::ComponentSignature sig = ecs.GetComponentSignature(entity);

	PushID(entity);
	Checkbox("##IsActive", &entity.m_active);
	SameLine();
	InputText("##Name", &entity.m_name);
	Text(("Entity ID: " + std::to_string(entity)).c_str());

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
					
					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					TableNextRow();
					vec3 temp = trans->m_pos;
					if (InputDouble3("Position", temp, inputWidth)) { trans->m_pos = temp; };
					TableNextRow();
					temp = trans->m_scale;
					if (InputDouble3("Scale", temp, inputWidth)) { trans->m_scale = temp; };
					TableNextRow();
					temp = trans->m_rot;
					if (InputDouble3("Rotation", temp, inputWidth)) { trans->m_rot = temp; };

					TableNextRow();
					InputDouble3("World Position", trans->m_worldPos, inputWidth, true);
					TableNextRow();
					InputDouble3("World Scale", trans->m_worldScale, inputWidth, true);
					TableNextRow();
					InputDouble3("World Rotation", trans->m_worldRot, inputWidth, true);

					EndTable();
					Separator();
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::BOX_COLLIDER:
			{
				auto col = ecs.GetComponent<BoxCollider>(entity);
				if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Separator();
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble1("Width", col->m_width);
					TableNextRow();
					InputDouble1("Height", col->m_height);
					TableNextRow();
					InputCheckBox("Show Collider", col->m_render);
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
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
			{
				//auto anim = ecs.GetComponent<SpriteAnim>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::MODEL:
			{
				//auto model = ecs.GetComponent<Model>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::TWEEN:
			{
				// Honestly no idea why -30 makes all 3 input fields match in size but sure
				float inputWidth = (contentSize - charSize - 30) / 3;

				auto tween = ecs.GetComponent<Tween>(entity);
				if (ImGui::CollapsingHeader("Tween", ImGuiTreeNodeFlags_DefaultOpen))
				{
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
				break;
			}
			case GE::ECS::COMPONENT_TYPES::DRAGGABLE:
			{
				CollapsingHeader("Draggable", ImGuiTreeNodeFlags_Leaf);
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
		Text(propertyName.c_str());
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
		Text(propertyName.c_str());
		TableNextColumn();
		SetNextItemWidth(GetWindowSize().x);
		InputDouble(("##" + propertyName).c_str(), &property, 0, 0, "%.2f");
		EndDisabled();
	}
	
	void InputCheckBox(std::string propertyName, bool& property, bool disabled)
	{
		BeginDisabled(disabled);
		TableNextColumn();
		Text(propertyName.c_str());
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
