#include <pch.h>
#include "AssetBrowser.h"
#include <ImGui/imgui.h>
#include <filesystem>
#include <AssetManager/AssetManager.h>

using namespace ImGui;
using namespace GE::Assets;

namespace
{
	void Traverse(std::filesystem::path filepath, ImColor textClr);
}

void GE::EditorGUI::AssetBrowser::CreateContent()
{
	AssetManager& assetManager = AssetManager::GetInstance();

	// Get style text colour that can be edited later
	ImGuiStyle& style = GetStyle();
	ImColor originalTextClr = style.Colors[ImGuiCol_Text];

	std::filesystem::path assetsDirectory{ *assetManager.GetConfigData<std::string>("Assets Dir") };
	if (!std::filesystem::exists(assetsDirectory))
	{
		throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Assets Directory not found! Path: " + assetsDirectory.string()));
	}

	//main node
	if (TreeNodeEx("Assets", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		//loop through files in Asset directory
		for (const auto& file : std::filesystem::directory_iterator(assetsDirectory))
		{
			if (!file.is_regular_file()) //if file is a folder i.e. directory
			{
				//create children nodes
				Traverse(file.path(), originalTextClr);
			}
		}
		TreePop();
	}
}

namespace
{
	void Traverse(std::filesystem::path filepath, ImColor textClr)
	{
		ImGuiStyle& style = GetStyle();
		style.Colors[ImGuiCol_Text] = textClr;

		if (TreeNodeEx(filepath.filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
		{
			//Create child nodes
			if (!std::filesystem::exists(filepath))
			{
				throw GE::Debug::Exception<AssetManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Directory not found! Path: " + filepath.string()));
			}

			for (const auto& file : std::filesystem::directory_iterator(filepath))
			{
				if (!file.is_regular_file()) //if file is a folder i.e. directory
				{
					//create children nodes
					Traverse(file.path(), textClr);
				}
			}
			TreePop();
		}
	}
}