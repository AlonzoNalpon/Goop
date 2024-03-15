/*!*********************************************************************
\file   GoopUtils.cpp
\author loh.j\@digipen.edu
\date   3-November-2023
\brief
	Contains some helper functions to allow readability for functions
	dealing with filepaths.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <GameStateManager/GameStateManager.h>
#include "GoopUtils.h"
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#endif

namespace GE::GoopUtils 
{
	std::vector<std::string> SplitStringByPipe(const std::string& input) {
		std::istringstream iss(input);
		std::vector<std::string> tokens;
		std::string token;

		while (std::getline(iss, token, '|')) {
			tokens.push_back(token);
		}

		return tokens;
	}

	void ReloadFileData()
	{
		auto& am{ GE::Assets::AssetManager::GetInstance() };
		am.ClearConfigData();
		am.LoadConfigData("./Assets/Config.cfg");
		am.ReloadAllFiles();
#ifndef IMGUI_DISABLE
		Prefabs::PrefabManager::GetInstance().ReloadPrefabs();
#endif
	}

  std::string GetFileExtension(const std::string& filePath)
  {
    size_t dotPosition = filePath.find_last_of('.');
    if (dotPosition != std::string::npos) {
      return filePath.substr(dotPosition + 1);
    }
    return ""; // No extension found
  }

	std::string ExtractPrevFolderAndFileName(const std::string& filepath)
	{
		size_t pos = filepath.find_last_of('/');

		std::string filename = (pos != std::string::npos) ? filepath.substr(pos + 1) : filepath;
		return filename;
	}

	std::string ExtractFilename(const std::string& filepath)
	{
		// Find the last occurrence of the path separator, typically '/'
		size_t pos = filepath.find_last_of('/');

		std::string filename = (pos != std::string::npos) ? filepath.substr(pos + 1) : filepath;

		// If the path separator is not found, try '\\'
		pos = filename.find_last_of('\\');

		// Extract the substring after the last path separator
		if (pos != std::string::npos)
		{
			filename = filename.substr(pos + 1);
		}

		// Find the last occurrence of the file extension separator, typically '.'
		size_t extPos = filename.find_last_of('.');

		// If the file extension separator is found, remove the extension
		if (extPos != std::string::npos) {
			filename = filename.substr(0, extPos);
		}

		return filename;
	}
}

