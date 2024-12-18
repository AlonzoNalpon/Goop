/*!*********************************************************************
\file   AssetPreview.cpp
\author loh.j\@digipen.edu
\date   26-November-2023
\brief
	Allows user to view assets from the asset browser.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "AssetPreview.h"

// Disable reinterpret to larger size
#pragma warning(disable : 4312)

void GE::EditorGUI::AssetPreview::CreateContent()
{
	auto& am = GE::Assets::AssetManager::GetInstance();
	unsigned w, h;
	float newW, newH;
	constexpr float WINDOW_SCALE{ 0.85f };
	float yPadding{};
	std::filesystem::path const& selectedAsset{ ImGuiHelper::GetSelectedAsset() };

	// if empty or not image, return
	if (selectedAsset.empty() || am.ImageFileExt.find(selectedAsset.extension().string()) == std::string::npos)
		return;
	
	am.GetDimensions(am.GetID(selectedAsset.string()), w, h);

	if (w == 0 || h == 0)
	{
		return;
	}

	if (w >= h)
	{
		newH = static_cast<float>(h) / static_cast<float>(w) * ImGui::GetWindowSize().x * WINDOW_SCALE;
		newW = ImGui::GetWindowSize().x * WINDOW_SCALE;
		yPadding = ImGui::GetCursorPosY() + (ImGui::GetWindowSize().y * 0.5f) - (newH * 0.5f);

		if (newH >= ImGui::GetWindowSize().y * WINDOW_SCALE)
		{
			newW = static_cast<float>(w) / static_cast<float>(h) * ImGui::GetWindowSize().y * WINDOW_SCALE;
			newH = ImGui::GetWindowSize().y * WINDOW_SCALE;
			yPadding = ImGui::GetCursorPosY();
		}
	}
	else
	{
		newW = static_cast<float>(w) / static_cast<float>(h) * ImGui::GetWindowSize().y * WINDOW_SCALE;
		newH = ImGui::GetWindowSize().y * WINDOW_SCALE;
		yPadding = ImGui::GetCursorPosY();
	}

	ImGui::SetCursorPos({ ImGui::GetCursorPosX() + (ImGui::GetWindowSize().x * 0.5f) - (newW * 0.5f) , yPadding });
	ImGui::Image(reinterpret_cast<ImTextureID>(am.GetID(selectedAsset.string())), { newW, newH }, { 0 ,1 }, { 1, 0 });
}
#endif
