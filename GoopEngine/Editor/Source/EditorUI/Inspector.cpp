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

// Disable empty control statement warning
#pragma warning(disable : 4390)

using namespace ImGui;
using namespace GE::Component;

namespace
{
	void InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled = false);
	void InputDouble1(std::string propertyName, double& property, bool disabled = false);
	void InputCheckBox(std::string propertyName, bool& property, bool disabled = false);

	template <typename T>
	void InputList(std::vector<T>& list, float fieldWidth, bool disabled = false);

	template <>
	void InputList(std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled);
}

void GE::EditorGUI::Inspector::CreateContent()
{
	GE::ECS::Entity entity = ImGuiHelper::GetSelectedEntity();

	if (entity == GE::ECS::INVALID_ID)
		return;

	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	GE::ECS::ComponentSignature sig = ecs.GetComponentSignature(entity);

	bool isActive{ecs.GetIsActiveEntity(entity)};
	if (Checkbox("##", &isActive))
	{
		ecs.SetIsActiveEntity(entity, isActive);
	}
	SameLine(); Text(ecs.GetEntityName(entity).c_str());
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
					InputDouble3("Position", trans->m_pos, inputWidth);					
					TableNextRow();
					InputDouble3("Scale", trans->m_scale, inputWidth);
					TableNextRow();
					InputDouble3("Rotation", trans->m_rot, inputWidth);
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
					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					InputDouble3("Velocity", vel->m_vel, inputWidth, true);
					TableNextRow();
					InputDouble3("Accelaration", vel->m_acc, inputWidth, true);
					TableNextRow();
					InputDouble3("Gravity", vel->m_gravity, inputWidth);
					TableNextRow();
					InputDouble3("Drag", vel->m_dragForce.m_magnitude, inputWidth);
					TableNextRow();
					InputCheckBox("Drag Active", vel->m_dragForce.m_isActive);
					TableNextRow();
					InputList(vel->m_forces, inputWidth);
					EndTable();
					Separator();
				}
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE:
			{
				//auto trans = ecs.GetComponent<Sprite>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SPRITE_ANIM:
			{
				//auto trans = ecs.GetComponent<SpriteAnim>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::MODEL:
			{
				//auto trans = ecs.GetComponent<Model>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::TWEEN:
			{
				//auto trans = ecs.GetComponent<Tween>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
			{
				//auto trans = ecs.GetComponent<ScriptHandler>(entity);
				break;
			}
			default:
				GE::Debug::ErrorLogger::GetInstance().LogWarning("Trying to serialize a component that is not being handled");
				break;
			}
		}
	}
}

namespace
{
	void InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth, bool disabled)
	{
		BeginDisabled(disabled);
		TableNextColumn();
		Text(propertyName.c_str());
		propertyName = "##" + propertyName;
		TableNextColumn();
		SetNextItemWidth(fieldWidth);
		InputDouble((propertyName + "X").c_str(), &property.x, 0, 0, "%.2f");
		SameLine(0, 3); SetNextItemWidth(fieldWidth); InputDouble((propertyName + "Y").c_str(), &property.y, 0, 0, "%.2f");
		SameLine(0, 3); SetNextItemWidth(fieldWidth); InputDouble((propertyName + "Z").c_str(), &property.z, 0, 0, "%.2f");
		EndDisabled();
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
	void InputList(std::vector<GE::Component::LinearForce>& list, float fieldWidth, bool disabled)
	{
		BeginDisabled(disabled);
		TableNextColumn();
		if (TreeNode("Forces"))
		{
			Separator();
			TableNextRow();
			int i{};
			for (auto& force : list)
			{
				InputDouble3("Force " + std::to_string(i++), force.m_magnitude, fieldWidth, disabled);
				InputDouble1("Lifetime", force.m_lifetime);
				InputCheckBox("Force Active", force.m_isActive);
				Separator();
			}
			TreePop();
		}
		EndDisabled();
	}
}
