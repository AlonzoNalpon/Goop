#include <pch.h>
#include <GameStateManager/GameStateManager.h>

namespace GE::GoopUtils 
{

	void ReloadFileData()
	{
		auto& am{ GE::Assets::AssetManager::GetInstance() };
		auto& of{ GE::ObjectFactory::ObjectFactory::GetInstance() };
		am.LoadConfigData("./Assets/Config.cfg");
		am.LoadFiles();
		of.LoadPrefabsFromFile();
	}

  std::string GetFileExtension(const std::string& filePath)
  {
    size_t dotPosition = filePath.find_last_of('.');
    if (dotPosition != std::string::npos) {
      return filePath.substr(dotPosition + 1);
    }
    return ""; // No extension found
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

