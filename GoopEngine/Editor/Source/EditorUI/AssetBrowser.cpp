/*!*********************************************************************
\file   AssetBrowser.cpp
\author c.phua\@digipen.edu
\co-authors
				loh.j\@digipen.edu
\date   3-November-2023
\brief
	Allows user to import assets/modify entities with files.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "AssetBrowser.h"
#include <AssetManager/AssetManager.h>
#include <ObjectFactory/ObjectFactory.h>
#include <filesystem>
#include<Graphics/GraphicsEngine.h>
#include <stb_image.h>
#include <commdlg.h>	// to open file explorer

using namespace ImGui;
using namespace GE::Assets;
using namespace GE::EditorGUI;

std::set<ImTextureID> GE::EditorGUI::AssetBrowser::m_textID;
std::set<int> GE::EditorGUI::AssetBrowser::m_assetIDs;
std::vector<int> GE::EditorGUI::AssetBrowser::toUnload;


namespace
{
	std::filesystem::path m_currDir;
	std::filesystem::path assetsDirectory;
	std::filesystem::path m_draggedPrefab;
	std::string const m_audioFile{ ".wav" }, m_imageFile{ ".png" }, m_shaderFile{ ".vert.frag" }, m_prefabFile{ ".pfb" }, m_sceneFile{ ".scn" }, m_fontFile{ ".otf" };
	float thumbnailSize = 300.0f;
	int colAmount = 5;
}

void AssetBrowser::CreateContentDir()
{
	AssetManager& assetManager = AssetManager::GetInstance();
	GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };

	if (Button("Reload Library"))
	{
		std::cout << "Reload Asset Browser" << std::endl;
		assetManager.LoadConfigData("./Assets/Config.cfg");
		assetManager.LoadFiles();
		of.LoadPrefabsFromFile();
	}

	assetsDirectory = assetManager.GetConfigData<std::string>("Assets Dir");
	if (!std::filesystem::exists(assetsDirectory))
	{
		throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Assets Directory not found! Path: " + assetsDirectory.string()));
	}

	//main node
	if (TreeNodeEx("Assets", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
	{
		if (IsItemClicked())
		{
			m_currDir = assetsDirectory;
			//AssetBrowser::InitView();
		}
		//loop through files in Asset directory
		for (const auto& file : std::filesystem::directory_iterator(assetsDirectory))
		{
			if (!file.is_regular_file()) //if file is a folder i.e. directory
			{
				//create children nodes
				Traverse(file.path());
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

	if (m_currDir == assetsDirectory)
	{
		Text("Assets");
		Separator();
	}
	else
	{
		Text(m_currDir.filename().string().c_str());
		Separator();
	}

	AssetManager& assetManager = AssetManager::GetInstance();
	BeginGroup();
	for (const auto& file : std::filesystem::directory_iterator(m_currDir))
	{
		std::string const& extension{ file.path().extension().string() };

		if (!file.is_regular_file())
		{
			continue;
		}

		std::string path = file.path().filename().string();
		const char* pathCStr = path.c_str();
		ImTextureID img = reinterpret_cast<ImTextureID>(assetManager.GetID(file.path().string()));

		if (extension == m_imageFile)
		{
			BeginGroup();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImVec2 imgsize{ 100, 100 };
			ImGui::ImageButton(img, imgsize, { 0, 1 }, { 1, 0 });
			if (IsItemClicked())
			{
				ImGuiHelper::SetSelectedAsset(file.path().string());
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				ImGui::SetWindowFocus("Asset Preview");
			}
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_IMAGE", pathCStr, strlen(pathCStr) + 1);
				Image(reinterpret_cast<ImTextureID>(assetManager.GetID(file.path().string())), { 50, 50 }, { 0, 1 }, { 1, 0 });
				Text(pathCStr);

				ImGui::EndDragDropSource();
			}
			Text(pathCStr);
			ImGui::PopStyleColor();
			float remainingsize = GetContentRegionMax().x - (GetCursorPosX() + imgsize.x);
			EndGroup();
			
			if (remainingsize > imgsize.x)
			{
				SameLine();
			}
		}
		else if (extension == m_prefabFile)
		{
			Selectable(pathCStr);
			if (IsItemClicked())
				ImGuiHelper::SetSelectedAsset("");
			
			if (ImGui::BeginDragDropSource())
			{
				m_draggedPrefab = file.path();
				ImGui::SetDragDropPayload("ASSET_BROWSER_PREFAB", &m_draggedPrefab, sizeof(std::filesystem::path));
				Text(pathCStr);
				ImGui::EndDragDropSource();
			}
		}
		else if (assetManager.FontFileExt.find(extension) != std::string::npos)
		{
			Selectable(pathCStr);
			if (IsItemClicked())
				ImGuiHelper::SetSelectedAsset("");

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_FONT", pathCStr, strlen(pathCStr) + 1);
				Text(pathCStr);
				ImGui::EndDragDropSource();
			}
		}
		else if (assetManager.SceneFileExt.find(extension) != std::string::npos)
		{
			Selectable(pathCStr);
			if (IsItemClicked())
				ImGuiHelper::SetSelectedAsset("");

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_SCENE", pathCStr, strlen(pathCStr) + 1);
				Text(pathCStr);
				ImGui::EndDragDropSource();
			}
		}
		else if (assetManager.AudioFileExt.find(extension) != std::string::npos)
		{
			Selectable(pathCStr);
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_AUDIO", pathCStr, strlen(pathCStr) + 1);
				Text(pathCStr);
				ImGui::EndDragDropSource();
			}
		}
		else
		{
			Text(pathCStr);
			/*if (ImGui::BeginDragDropSource())
			{
				if (extension == m_prefabFile ||
					extension == m_imageFile ||
					assetManager.FontFileExt.find(extension) != std::string::npos)
				{
					ImGui::SetDragDropPayload("ASSET_BROWSER_IMAGE", pathCStr, strlen(pathCStr) + 1);
					Text(pathCStr);
				}
				ImGui::EndDragDropSource();
			}*/
		}
	}
	EndGroup();
}

void GE::EditorGUI::AssetBrowser::CreateContent()
{
	// 30 characters for file name
	float charSize = CalcTextSize("01234567890").x * 2;
	if (BeginTable("##", 2, ImGuiTableFlags_BordersInnerV))
	{
		TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, charSize);
		TableNextColumn();
		CreateContentDir();
		TableNextColumn();
		CreateContentView();
		EndTable();
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

	//create temp vcector/list of filenames that you are loading -> add in whatever isnt alr loaded (i.e. only loaded for browser)

	// std::vector<id> filesToLoad;
	// for (filesToLoad) [
	//		if (!alreadyLoaded())
	//			call load function
	//			filesToLoad.emplace_back(filename);
	//		
	// }


	// for (filesToLoad) [ free(getname(id)); }

	AssetManager& assetManager = AssetManager::GetInstance();
	auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;

	if (!m_textID.empty())
	{
		for (auto itr : toUnload)
		{
			assetManager.FreeImage(GE::GoopUtils::ExtractFilename(assetManager.GetName(itr)));
		}

		m_textID.clear();
		toUnload.clear();
	}

	for (const auto& file : std::filesystem::directory_iterator(m_currDir.string()))
	{
		if (!file.is_regular_file()) //if file is a folder, continue -> don't need to load image
		{
			continue;
		}

		std::string const& extension{ file.path().extension().string() }; //getting extension on file
		if (extension != m_imageFile) //if file is not an image file, continue -> don't need to load image
		{
			continue;
		}

		unsigned id{ 0 };
		if (!assetManager.AlreadyLoaded(file.path().string()))
		{
			id = assetManager.LoadImageW(file.path().string());
			toUnload.emplace_back(id);
		}
		id = assetManager.LoadImageW(file.path().string());
		m_assetIDs.emplace(id);
	}

	for (auto const& itr : m_assetIDs)
	{
		auto imgID = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(assetManager.GetName(itr)));
		m_textID.insert(reinterpret_cast<ImTextureID>(imgID));
	}
}

void AssetBrowser::Traverse(std::filesystem::path filepath)
{
	int folderCnt{};
	std::filesystem::directory_iterator countIter(filepath);
	for (auto const& file : countIter)
	{
		folderCnt += (file.is_directory()) ? 1 : 0;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= (folderCnt == 0) ? ImGuiTreeNodeFlags_Leaf : 0;

	if (TreeNodeEx(filepath.filename().string().c_str(), flags))
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
				Traverse(file.path());
			}
		}
		TreePop();
	}
}

std::string AssetBrowser::GetRelativeFilePath(std::string const& filepath, std::string const& rootDir)
{
	return "." + filepath.substr(filepath.find(rootDir) + rootDir.size());
}

std::string AssetBrowser::LoadFileFromExplorer(const char* extensionsFilter, unsigned numFilters, const char* initialDir)
{
	OPENFILENAMEA fileName{};
	CHAR size[MAX_PATH]{};
	
	ZeroMemory(&fileName, sizeof(fileName));
	fileName.lStructSize = sizeof(fileName);
	fileName.hwndOwner = NULL;
	fileName.lpstrFile = size;
	fileName.nMaxFile = sizeof(size);
	fileName.lpstrFilter = extensionsFilter;
	fileName.nFilterIndex = numFilters;			// number of filters
	fileName.lpstrFileTitle = NULL;
	fileName.nMaxFileTitle = 0;
	fileName.lpstrInitialDir = initialDir;	// initial directory
	fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&fileName))
	{
		return GetRelativeFilePath(fileName.lpstrFile);
	}

	throw GE::Debug::Exception<AssetBrowser>::Exception(GE::Debug::LEVEL_ERROR, ErrMsg("Unable to open file"));
}

std::string AssetBrowser::SaveFileToExplorer(const char* extensionsFilter, unsigned numFilters, const char* initialDir)
{
	OPENFILENAMEA fileName{};
	CHAR size[MAX_PATH]{};

	ZeroMemory(&fileName, sizeof(fileName));
	fileName.lStructSize = sizeof(fileName);
	fileName.hwndOwner = NULL;
	fileName.lpstrFile = size;
	fileName.nMaxFile = sizeof(size);
	fileName.lpstrFilter = extensionsFilter;
	fileName.nFilterIndex = numFilters;			// number of filters
	fileName.lpstrFileTitle = NULL;
	fileName.nMaxFileTitle = 0;
	fileName.lpstrInitialDir = initialDir;	// initial directory
	// OFN_NOCHANGEDIR specifies not to change the working dir
	fileName.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&fileName))
	{
		std::string const newFile{ GetRelativeFilePath(fileName.lpstrFile) };
		std::ofstream ofs{ newFile };
		if (ofs)
		{
			ofs.close();
			return newFile;
		}
	}

	throw GE::Debug::Exception<AssetBrowser>::Exception(GE::Debug::LEVEL_ERROR, ErrMsg("Unable to save file"));
}

