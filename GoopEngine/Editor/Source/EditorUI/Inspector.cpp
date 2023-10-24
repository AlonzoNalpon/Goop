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
	void InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth);
	void InputDouble1(std::string propertyName, double& property);
}

void GE::EditorGUI::Inspector::CreateContent()
{
	GE::ECS::Entity entity = ImGuiHelper::GetSelectedEntity();

	if (entity == GE::ECS::INVALID_ID)
		return;

	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	GE::ECS::ComponentSignature sig = ecs.GetComponentSignature(entity);

	for (int i{}; i < GE::ECS::MAX_COMPONENTS; ++i)
	{
		if (sig[i])		
		{
			switch (static_cast<GE::ECS::COMPONENT_TYPES>(i))
			{
			case GE::ECS::COMPONENT_TYPES::TRANSFORM:
			{
				auto trans = ecs.GetComponent<Transform>(entity);
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Separator();
					float contentSize = GetWindowSize().x;
					// 10 characters
					float charSize = CalcTextSize("0123456789").x;
					// Honestly no idea why -30 makes all 3 input fields match in size but sure
					float inputWidth = (contentSize - charSize - 30) / 3;

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
				//auto trans = ecs.GetComponent<BoxCollider>(entity);
				break;
			}
			case GE::ECS::COMPONENT_TYPES::VELOCITY:
			{
				//auto trans = ecs.GetComponent<Velocity>(entity);
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
	void InputDouble3(std::string propertyName, GE::Math::dVec3& property, float fieldWidth)
	{
		TableNextColumn();
		Text(propertyName.c_str());
		TableNextColumn();
		SetNextItemWidth(fieldWidth);
		InputDouble(("##" + propertyName + "X").c_str(), &property.x, 0, 0, "%.2f");
		SameLine(0, 3); SetNextItemWidth(fieldWidth); InputDouble(("##" + propertyName + "Y").c_str(), &property.y, 0, 0, "%.2f");
		SameLine(0, 3); SetNextItemWidth(fieldWidth); InputDouble(("##" + propertyName + "Z").c_str(), &property.z, 0, 0, "%.2f");
	}

	void InputDouble1(std::string propertyName, double& property)
	{
		TableNextColumn();
		Text(propertyName.c_str());
		TableNextColumn();
		SetNextItemWidth(GetWindowSize().x);
		InputDouble(("##" + propertyName).c_str(), &property, 0, 0, "%.2f");
	}
}
