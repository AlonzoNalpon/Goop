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
				// Can use InputFloat3 also
				auto trans = ecs.GetComponent<Transform>(entity);
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float contentSize = GetWindowSize().x;
					// 10 characters
					float charSize = CalcTextSize("0123456789").x;
					float inputWidth = (contentSize - charSize) / 3;

					//Text("Position");
					//SameLine(contentSize.x - (inputWidth * 3));
					//SetNextItemWidth(inputWidth);
					//if (InputDouble("##X", &trans->m_pos.x, 0, 0, "%.2f"));
					//SameLine();
					//SetNextItemWidth(inputWidth);
					//if (InputDouble("##Y", &trans->m_pos.y, 0, 0, "%.2f"));
					//SameLine();
					//SetNextItemWidth(inputWidth);
					//if (InputDouble("##Z", &trans->m_pos.z, 0, 0, "%.2f"));

					BeginTable("##", 2, ImGuiTableFlags_BordersInnerV);
					ImGui::TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
					// Not sure why my 3rd Input size is smaller

					TableNextColumn();
					Text("Position");
					TableNextColumn();
					SetNextItemWidth(inputWidth);
					InputDouble("##TX", &trans->m_pos.x, 0, 0, "%.2f");
					SameLine(0, 3); SetNextItemWidth(inputWidth); InputDouble("##TY", &trans->m_pos.y, 0, 0, "%.2f");
					SameLine(0, 3); SetNextItemWidth(inputWidth); InputDouble("##TZ", &trans->m_pos.z, 0, 0, "%.2f");
					
					TableNextRow();
					TableNextColumn();
					Text("Scale");
					TableNextColumn();
					SetNextItemWidth(inputWidth); 
					InputDouble("##SX", &trans->m_scale.x, 0, 0, "%.2f");
					SameLine(0, 3); SetNextItemWidth(inputWidth); InputDouble("##SY", &trans->m_scale.y, 0, 0, "%.2f");
					SameLine(0, 3); SetNextItemWidth(inputWidth); InputDouble("##SZ", &trans->m_scale.z, 0, 0, "%.2f");

					TableNextRow();
					TableNextColumn();
					Text("Rotation");
					TableNextColumn();
					SetNextItemWidth(GetWindowSize().x);
					InputDouble("##R", &trans->m_rot, 0, 0, "%.2f");

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
