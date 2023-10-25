#pragma once
#include "ImGuiUI.h"
#include <ImGui/imgui.h>

namespace GE::EditorGUI
{
	class AssetBrowser
	{
	private:
		static std::set<ImTextureID> m_textID;
		static std::set<int> m_assetIDs;

		static void Traverse(std::filesystem::path filepath, ImColor textClr);

	public:
		static void CreateContentDir();
		static void CreateContentView();
		static void InitView();
	};
}