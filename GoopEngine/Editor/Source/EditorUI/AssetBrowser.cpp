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
#ifndef IMGUI_DISABLE
#include "AssetBrowser.h"
#include <AssetManager/AssetManager.h>
#include <ObjectFactory/ObjectFactory.h>
#include <filesystem>
#include<Graphics/GraphicsEngine.h>
#include <stb_image.h>
#include <commdlg.h>	// to open file explorer
#include <Events/EventManager.h>
#include "PrefabEditor.h"
#include <Systems/GameSystem/GameSystem.h>
#include <shellapi.h>
#include <GameStateManager/GameStateManager.h>

using namespace ImGui;
using namespace GE::Assets;
using namespace GE::EditorGUI;

std::set<ImTextureID> GE::EditorGUI::AssetBrowser::m_textID;
std::set<int> GE::EditorGUI::AssetBrowser::m_assetIDs;
std::vector<int> GE::EditorGUI::AssetBrowser::toUnload;
bool AssetBrowser::m_assetsMenuOpen{ false };

namespace
{
	std::filesystem::path m_currDir;
	std::filesystem::path assetsDirectory;
	std::filesystem::path m_draggedPrefab;
}

void AssetBrowser::ClearContent()
{
	m_currDir.clear();
}

void AssetBrowser::CreateContentDir()
{
	AssetManager& assetManager = AssetManager::GetInstance();
	GE::ObjectFactory::ObjectFactory& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };

	if (Button("Reload Library"))
	{
		//  << "Reload Asset Browser" << std::endl;
		GE::Debug::ErrorLogger::GetInstance().LogError("=== [ Reloading Asset Browser ] ===");


		Events::EventManager::GetInstance().Dispatch(Events::StartSceneEvent());
		GoopUtils::ReloadFileData();
		Events::EventManager::GetInstance().Dispatch(Events::StopSceneEvent());
		GE::GSM::GameStateManager::GetInstance().Restart();
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
	for (const auto& file : std::filesystem::recursive_directory_iterator(m_currDir))
	{
		if (!file.is_regular_file())
		{
			continue;
		}

		std::string const extension{ file.path().extension().string() };

		std::string const path = file.path().filename().string();
		const char* pathCStr = path.c_str();

		if (assetManager.ImageFileExt.find(extension) != std::string::npos)
		{
			try
			{
				ImTextureID img = reinterpret_cast<ImTextureID>(assetManager.GetID(file.path().string()));
				BeginGroup();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImVec2 imgsize{ 100, 100 };
				ImGui::ImageButton(img, imgsize, { 0, 1 }, { 1, 0 });
				if (IsItemClicked())
				{
					ImGuiHelper::SetSelectedAsset(GoopUtils::ExtractPrevFolderAndFileName(file.path().string()));
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
			catch (GE::Debug::IExceptionBase&)
			{
				continue;
			}
		}
		else if (assetManager.PrefabFileExt.find(extension) != std::string::npos)
		{
			Selectable(pathCStr);
			
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

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_SCENE", pathCStr, strlen(pathCStr) + 1);
				Text(pathCStr);
				ImGui::EndDragDropSource();
			}
		}
		else if (assetManager.AudioFileExt.find(extension) != std::string::npos)
		{
			std::string filename = path.substr(0, path.find_last_of('.'));
			const char* fileCStr = filename.c_str();
			Selectable(pathCStr);
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_BROWSER_AUDIO", fileCStr, strlen(pathCStr) + 1);
				Text(fileCStr);
				ImGui::EndDragDropSource();
			}
		}
		else
		{
			Selectable(pathCStr);
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGuiHelper::SetSelectedAsset(file);
			OpenPopup("AssetsMenu");
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			OpenFileWithDefaultProgram(file.path().string());
		}
	}

	static bool deletePopup{ false };
	if (BeginPopup("AssetsMenu"))
	{
		// only enabled for prefabs
		if (ImGuiHelper::GetSelectedAsset().extension().string() == ".pfb")
		{
			ImGui::BeginDisabled(GE::EditorGUI::PrefabEditor::IsEditingPrefab());
			if (Selectable("Edit Prefab"))
			{
				Events::EventManager::GetInstance().Dispatch(Events::EditPrefabEvent(
					ImGuiHelper::GetSelectedAsset().stem().string(),
					ImGuiHelper::GetSelectedAsset().relative_path().string())
				);
			}
			ImGui::EndDisabled();
		}

		if (Selectable("Delete"))
		{
			deletePopup = true;
		}

		EndPopup();
	}

	if (deletePopup)
	{
		ImGui::OpenPopup("Confirm Delete");
		deletePopup = false;
	}
	RunConfirmDeletePopup();

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
		if (assetManager.ImageFileExt.find(extension) == std::string::npos) //if file is not an image file, continue -> don't need to load image
		{
			continue;
		}

		unsigned id{ 0 };
		if (!assetManager.AlreadyLoaded(GoopUtils::ExtractPrevFolderAndFileName(file.path().string())))
		{
			id = assetManager.LoadImageW(file.path().string());
			toUnload.emplace_back(id);
		}
		id = assetManager.LoadImageW(file.path().string());
		m_assetIDs.emplace(id);
	}

	for (auto const& itr : m_assetIDs)
	{
		try
		{
			auto imgID = texManager.GetTextureID(GE::GoopUtils::ExtractFilename(assetManager.GetName(itr)));
			m_textID.insert(reinterpret_cast<ImTextureID>(imgID));
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
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
			TreePop();
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

void AssetBrowser::RunConfirmDeletePopup()
{
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete");
		ImGui::SameLine();
		auto const& asset{ ImGuiHelper::GetSelectedAsset() };
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), asset.filename().string().c_str());
		ImGui::SameLine();
		ImGui::Text("?");

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - ImGui::CalcTextSize("Yes ").x);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.65f, 0.f, 1.f));
		if (ImGui::Button("No"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.f));
		if (ImGui::Button("Yes"))
		{
			Assets::AssetManager const& am{ Assets::AssetManager::GetInstance() };
			// if prefab deleted
			if (asset.extension() == am.PrefabFileExt)
			{
				Events::EventManager::GetInstance().Dispatch(Events::DeletePrefabEvent(asset.stem().string()));
			}
			else
			{
				Assets::AssetType type{ Assets::AssetType::NONE };
				if (asset.extension() == am.SceneFileExt)
					type = Assets::AssetType::SCENE;
				else if (asset.extension() == am.ImageFileExt)
					type = Assets::AssetType::IMAGES;
				else if (asset.extension() == am.AudioFileExt)
					type = Assets::AssetType::AUDIO;
				else if (asset.extension() == am.FontFileExt)
					type = Assets::AssetType::FONTS;
				else if (asset.extension() == am.ShaderFileExt)
					type = Assets::AssetType::SHADERS;

				Events::EventManager::GetInstance().Dispatch(Events::DeleteAssetEvent(type, asset.stem().string()));
			}
			std::remove(asset.relative_path().string().c_str());
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();

		ImGui::EndPopup();
	}
}

void AssetBrowser::OpenFileWithDefaultProgram(std::string const& filePath)
{
	std::filesystem::path path{ filePath };
	std::string const absolutePath{ std::filesystem::absolute(filePath).string() };
	std::wstring wsStr{ absolutePath.begin(), absolutePath.end() };
	ShellExecute(0, 0, wsStr.c_str(), 0, 0, SW_SHOW);
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

#endif