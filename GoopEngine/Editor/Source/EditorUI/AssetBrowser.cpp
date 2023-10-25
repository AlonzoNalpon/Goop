#include <pch.h>
#include "AssetBrowser.h"
#include <AssetManager/AssetManager.h>
#include <filesystem>
#include<Graphics/GraphicsEngine.h>

using namespace ImGui;
using namespace GE::Assets;
using namespace GE::EditorGUI;

std::set<ImTextureID> GE::EditorGUI::AssetBrowser::m_textID;
std::set<int> GE::EditorGUI::AssetBrowser::m_assetIDs;
std::vector<int> GE::EditorGUI::AssetBrowser::toUnload;

namespace
{
	std::filesystem::path m_currDir;
	std::string const m_audioFile{ ".wav" }, m_imageFile{ ".png" }, m_shaderFile{ ".vert.frag" }, m_prefabFile{ ".pfb" }, m_sceneFile{ ".scn" };
}

void AssetBrowser::CreateContentDir()
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

void AssetBrowser::CreateContentView()
{
	//AssetManager& assetManager = AssetManager::GetInstance();

	if (m_currDir.empty())
	{
		return;
	}

	for (const auto& file : std::filesystem::directory_iterator(m_currDir))
	{
		std::string const& extension{ file.path().extension().string() };

		// in init:
		// free all ids loaded, if it exists
		// load new images in new currDir
		//assetManager.LoadDirectory();
		//will return a set of ints i.e. id for image loaded
		//texManager.GetTexture();
		//texManager.GetTextureID();
		// shld have container of IDs
		// 
		// in create content
		// ImGui::Image(for all ids)

		for (auto itr : m_textID)	// image
		{
			//print img using ImGui::Image();
			Image(itr, { 25.0f , 25.0f });
			//name of image
			Text(file.path().filename().string().c_str());
		}
		if (!file.is_regular_file())
		{
			continue;
		}
		else if (extension == m_audioFile)	// sound
		{
			//print img of maybe a audio file logo?
			//name of audio
			Text(file.path().filename().string().c_str());
		}
		//else if (extension == m_imageFile)	// prefab
		//{
		//	//name of prefab
		//	Text(file.path().filename().string().c_str());
		//}
		else if (extension == m_prefabFile)	// prefab
		{
			//name of prefab
			Text(file.path().filename().string().c_str());
		}
		else if (extension == m_sceneFile)	// scene
		{
			//name of scene
			Text(file.path().filename().string().c_str());
		}
		else if (m_shaderFile.find(extension) != std::string::npos)
		{
			//name of shader
			Text(file.path().filename().string().c_str());
		}
	}
}

void AssetBrowser::InitView()
{
	// in init:
	// free all ids loaded, if it exists~
	// load new images in new currDir by using:
	//assetManager.LoadDirectory();~
	//will return a set of ints i.e. id for image loaded~
	//texManager.GetTexture();
	//texManager.GetTextureID();
	// shld have container of IDs~

	AssetManager& assetManager = AssetManager::GetInstance();
	auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;

	if (!m_textID.empty())
	{
		for (const auto& i : m_textID)
		{
			std::cout << *reinterpret_cast<int*>(i) << std::endl;
		}
		for (auto itr : toUnload)
		{
			assetManager.FreeImage(assetManager.ExtractFilename(assetManager.GetName(itr)));
		}

		m_textID.clear();
		toUnload.clear();
	}

	for (const auto& entry : std::filesystem::directory_iterator(m_currDir.string()))
	{
		int id;
		if (!assetManager.AlreadyLoaded(entry.path().string()))
		{
			id = assetManager.LoadImageW(entry.path().string());
			toUnload.emplace_back(id);
		}
		id = assetManager.LoadImageW(entry.path().string());
		m_assetIDs.emplace(id);
	}

	for (auto itr : m_assetIDs)
	{
		auto imgID = texManager.GetTextureID(assetManager.ExtractFilename(assetManager.GetName(itr)));
		m_textID.insert(reinterpret_cast<ImTextureID>(&imgID));
	}
}

void AssetBrowser::Traverse(std::filesystem::path filepath, ImColor textClr)
{
	ImGuiStyle& style = GetStyle();
	style.Colors[ImGuiCol_Text] = textClr;

	if (TreeNodeEx(filepath.filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		//if folder is clicked
		if (IsItemClicked())
		{
			m_currDir = filepath;
			AssetBrowser::InitView();
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