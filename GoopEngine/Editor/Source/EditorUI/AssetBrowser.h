/*!*********************************************************************
\file   AssetBrowser.h
\author c.phua\@digipen.edu
\co-authors
				loh.j\@digipen.edu
\date   3-November-2023
\brief
	Allows user to import assets/modify entities with files.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include "ImGuiUI.h"
#include <ImGui/imgui.h>

namespace GE::EditorGUI
{
	class AssetBrowser : public Singleton<AssetBrowser>
	{
	private:
		static std::set<ImTextureID> m_textID;
		static std::set<int> m_assetIDs;
		static std::vector<int> toUnload;
		static bool m_assetsMenuOpen;

		/*!*********************************************************************
		\brief
			Traverses the directory passed in and created child nodes for directories within it
		\param filepath
			Parent directory i.e. current directory
		************************************************************************/
		static void Traverse(std::filesystem::path filepath);

		/*!*********************************************************************
		\brief
		  Creates the confirmation popup for when an asset is deleted
		************************************************************************/
		static void RunConfirmDeletePopup();

	public:

		void ClearContent();

		/*!*********************************************************************
		\brief
			Opens the file explorer and allows the user to select 1 or more files.
			The filepath relative to initialDir is returned.
		\param extensionsFilter
			The list of file extensions to limit the explorer to show
			Format:
				A null character-separated list of filter pairs where each
				element will consist of:
					1. Label of the file extension
					2. Semicolon separated list of file extensions accepted by the
						 filter

			Example:
				"Scenes (*.scn)\0*.scn\0All Files (*.*)\0*.*"
		\param numFilters
			The number of filters (should correspond to extensionsFilter)
		\param initialDir
			The filepath of the initial directory the file explorer opens up at.
			Defaults to Assets.
		\return
			The vector of filepaths selected
		************************************************************************/
		static std::vector<std::string> SelectFilesFromExplorer(const char* extensionsFilter, unsigned numFilters, const char* initialDir);

		/*!*********************************************************************
		\brief
		  Opens the file explorer and allows the user to select a file. The
			filepath relative to initialDir is returned.
		\param extensionsFilter
			The list of file extensions to limit the explorer to show
			Format:
				A null character-separated list of filter pairs where each 
				element will consist of:
					1. Label of the file extension
					2. Semicolon separated list of file extensions accepted by the
						 filter

			Example:
				"Scenes (*.scn)\0*.scn\0All Files (*.*)\0*.*"
		\param numFilters
			The number of filters (should correspond to extensionsFilter)
		\param initialDir
			The filepath of the initial directory the file explorer opens up at.
			Defaults to Assets.
		\return
		  The filepath of the file selected
		************************************************************************/
		static std::string LoadFileFromExplorer(const char* extensionsFilter = "All Files (*.*)\0*.*",
			unsigned numFilters = 1, const char* initialDir = "./Assets");

		/*!*********************************************************************
		\brief
			Opens the file explorer and allows the user to create a file. The
			filepath relative to initialDir is returned.
		\param extensionsFilter
			The list of file extensions to limit the explorer to show
			Format:
				A null character-separated list of filter pairs where each
				element will consist of:
					1. Label of the file extension
					2. Semicolon separated list of file extensions accepted by the
						 filter

			Example:
				"Scenes (*.scn)\0*.scn\0All Files (*.*)\0*.*"
		\param numFilters
			The number of filters (should correspond to extensionsFilter)
		\param initialDir
			The filepath of the initial directory the file explorer opens up at.
			Defaults to Assets.
		\return
			The filepath of the file selected
		************************************************************************/
		static std::string SaveFileToExplorer(const char* extensionsFilter = "All Files (*.*)\0*.*",
			unsigned numFilters = 1, const char* initialDir = "./Assets");

		/*!*********************************************************************
		\brief
			Opens the file explorer and allows the user to create a file. The
			filepath relative to initialDir is returned.
		\param filePath
			The path of the file
		\param numFilters
			The number of filters (should correspond to extensionsFilter)
		\param initialDir
			The filepath of the initial directory the file explorer opens up at.
			Defaults to Assets.
		************************************************************************/
		static void OpenFileWithDefaultProgram(std::string const& filePath);

		/*!*********************************************************************
		\brief
			Takes an absolute filepath and returns the relative file path 
			based on the root directory passed in
		\param filepath
			The absolute filepath
		\param rootDir
			The root directory that will be replaced with "./"
		\return
		  The filepath relative to the given root directory
		************************************************************************/
		static std::string GetRelativeFilePath(std::string const& filepath, std::string const& rootDir = "Editor");

		/*!*********************************************************************
		\brief
			Forms the directory for asset browser
		************************************************************************/
		static void CreateContentDir();

		/*!*********************************************************************
		\brief
			Forms the view for asset browser
		************************************************************************/
		static void CreateContentView();

		/*!*********************************************************************
		\brief
			Forms the content for both the directory and view for asset browser
		************************************************************************/
		static void CreateContent();

		/*!*********************************************************************
		\brief
			Frees IDs of the images that were from the previous initialisation.
			Then loads all images needed for current directory.
		************************************************************************/
		static void InitView();
	};
}
#endif