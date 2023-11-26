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
	
	if (ImGuiHelper::GetSelectedAsset() == "")
		return;
	
	am.GetDimensions(am.GetID(ImGuiHelper::GetSelectedAsset()), w, h);

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
	ImGui::Image(reinterpret_cast<ImTextureID>(am.GetID(ImGuiHelper::GetSelectedAsset())), { newW, newH }, { 0 ,1 }, { 1, 0 });
}
#endif
