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
		static std::vector<int> toUnload;

		static void Traverse(std::filesystem::path filepath);
		

	public:
		/*!*********************************************************************
		\brief
		  
		\param extensionsFilter
			The list of file extensions to limit the explorer to show
			Format:
				A null character-separated list of filter pairs where each 
				element will consist of:
					1. Label of the file extension
					2. Semicolon separated list of file extensions accepted by the
						 filter

			Example:
				"Scenes (*.scn)\0*.scn\0All Files (*.*)\0*.*\0"
		\param initialDir
			The filepath of the initial directory the file explorer opens up at.
			Defaults to Assets.
		\return
		  The filepath of the file selected
		************************************************************************/
		static std::string OpenFileExplorer(const char* extensionsFilter = "All Files (*.*)\0*.*\0",
			unsigned numFilters = 1, const char* initialDir = "./Assets");

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
		static std::string GetRelativeFilePath(std::string const& filepath, std::string const& rootDir = "GoopEngine");

		static void CreateContentDir();
		static void CreateContentView();
		static void CreateContent();
		static void InitView();
	};
}