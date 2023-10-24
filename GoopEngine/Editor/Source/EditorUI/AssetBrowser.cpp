#include <pch.h>
#include "AssetBrowser.h"
#include <AssetManager/AssetManager.h>
#include <ImGui/imgui.h>
#include <filesystem>

using namespace ImGui;
using namespace GE::Assets;

namespace
{
	std::filesystem::path m_currDir;
	std::string const m_audioFile{ ".wav" }, m_imageFile{ ".png" }, m_shaderFile{ ".vert.frag" }, m_prefabFile{ ".pfb" }, m_sceneFile{ ".scn" };

	void Traverse(std::filesystem::path filepath, ImColor textClr);
}

void GE::EditorGUI::AssetBrowser::CreateContentDir()
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

void GE::EditorGUI::AssetBrowser::CreateContentView()
{
	AssetManager& assetManager = AssetManager::GetInstance();

	if (m_currDir.empty())
	{
		return;
	}

	for (const auto& file : std::filesystem::directory_iterator(m_currDir))
	{
		std::string const& extension{ file.path().extension().string() };
		if (extension == m_imageFile)	// image
		{
			//print img
			ImGui::Image(assetManager.GetData());
			//name of image
		}
		else if (extension == m_audioFile)	// sound
		{
			//print img of maybe a audio file logo?
			//name of audio
		}
		else if (extension == m_prefabFile)	// prefab
		{
			//name of prefab
		}
		else if (extension == m_sceneFile)	// scene
		{
			//name of scene
		}
		else if (m_shaderFile.find(extension) != std::string::npos)
		{
			//name of shader
		}
		else
		{
			Debug::ErrorLogger::GetInstance().LogMessage("AssetBrowser: " + file.path().string() + " ignored & not in view.");
		}
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
			//if folder is clicked
			if (IsItemClicked())
			{
				m_currDir = filepath;
			}

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