/*!*********************************************************************
\file   AssetManager.cpp
\author loh.j\@digipen.edu
\date   13 September 2023
\brief
	Asset Manager is capable of:
		- LoadDirectory
				Loading all files located in directory

		- LoadImageW
				Loading specific image from path

		- GetData
				Get image data for graphics.

		- LoadJSONData
				Load Json files and save it into the class.
				Able to load Images, Config, and Animation json.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <AssetManager/AssetManager.h>
#include <filesystem>
#include <Graphics/GraphicsEngine.h>
#include <Serialization/GooStream/AssetGooStream.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#define ASSET_MANAGER_DEBUG
using namespace GE::Serialization;

namespace GE::Assets
{
	AssetManager::~AssetManager()
	{
		FreeImages();
	}

	std::string AssetManager::ExtractFilename(const std::string& filepath)
	{
		// Find the last occurrence of the path separator, typically '/'
		size_t pos = filepath.find_last_of('/');

		std::string filename = (pos != std::string::npos) ? filepath.substr(pos + 1) : filepath;

		// If the path separator is not found, try '\\'
		pos = filename.find_last_of('\\');

		// Extract the substring after the last path separator
		filename = (pos != std::string::npos) ? filename.substr(pos + 1) : filename;

		// Find the last occurrence of the file extension separator, typically '.'
		size_t extPos = filename.find_last_of('.');

		// If the file extension separator is found, remove the extension
		if (extPos != std::string::npos) {
			filename = filename.substr(0, extPos);
		}

		return filename;
	}

	const std::string& AssetManager::GetName(int id)
	{
		if (m_loadedImagesIDLookUp.find(id) == m_loadedImagesIDLookUp.end())
		{
#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND IMAGE DATA WITH ID: " << id << "\n";
#endif
			return m_loadedImagesIDLookUp[id];
		}

		return m_loadedImagesIDLookUp[id];
	}
	int AssetManager::GetID(const std::string& name)
	{
		if (m_loadedImagesStringLookUp.find(GoopUtils::ExtractPrevFolderAndFileName(name)) == m_loadedImagesStringLookUp.end())
		{
#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND ID WITH KEY: " << name << "\n";
#endif
			return m_loadedImagesStringLookUp[GoopUtils::ExtractPrevFolderAndFileName(name)];
		}

		return m_loadedImagesStringLookUp[GoopUtils::ExtractPrevFolderAndFileName(name)];

	}
	std::string AssetManager::GetScene(std::string const& sceneName)
	{
		if (m_scenes.find(sceneName) != m_scenes.end())
		{
			return m_scenes.at(sceneName);
		}

		// File missing, try to load
		std::filesystem::path assetsDir{ GetConfigData<std::string>("Assets Dir") };
		for (const auto& file : std::filesystem::recursive_directory_iterator(assetsDir))
		{
			if (!file.is_regular_file()) { continue; }	// skip if file is a directory

			std::string const& currExt{ file.path().extension().string() };
			if (SceneFileExt.find(currExt) != std::string::npos)	// scene
			{
				m_scenes.emplace(file.path().stem().string(), file.path().string());
				// File loaded so just call self to return newly loaded file
				return GetScene(sceneName);
			}
		}

		GE::Debug::ErrorLogger::GetInstance().LogError("Unable to load scene " + sceneName);
		return "";
	}

	std::string AssetManager::GetSound(std::string const& soundName)
	{
		if (m_audio.find(soundName) != m_audio.end())
		{
			return m_audio.at(soundName);
		}

		// File missing, try to load
		std::filesystem::path assetsDir{ GetConfigData<std::string>("Assets Dir") };
		for (const auto& file : std::filesystem::recursive_directory_iterator(assetsDir))
		{
			if (!file.is_regular_file()) { continue; }	// skip if file is a directory

			std::string const& fileName{ file.path().filename().string()};
			if (fileName == soundName)	// scene
			{
				m_audio[file.path().stem().string()] = file.path().string();
				// File loaded so just call self to return newly loaded file
				return GetSound(soundName);
			}
		}

		GE::Debug::ErrorLogger::GetInstance().LogError("Unable to load audio " + soundName);
		return "";
	}

	ImageData const& AssetManager::GetData(int id)
	{
		if (m_loadedImages.find(id) == m_loadedImages.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to get data: " + id));
		}

		return m_loadedImages[id];
	}
	ImageData const& AssetManager::GetData(const std::string& name)
	{
		if (m_loadedImages.find(GetID(name)) == m_loadedImages.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to get data: " + name));
		}

		return m_loadedImages[GetID(name)];
	}

	void AssetManager::LoadFonts()
	{
		constexpr GLint FONT_SIZE{ 256 };
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		for (auto entry : m_fonts)
		{
			gEngine.fontManager.LoadFont(GE::GoopUtils::ExtractFilename(entry.first), entry.second, FONT_SIZE);
		}
	}

	void AssetManager::LoadFiles()
	{
		// construct path object with relative path to project dir
		std::filesystem::path assetsDir{ GetConfigData<std::string>("Assets Dir") };
		if (!std::filesystem::exists(assetsDir))
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to open Assets Dir: " + assetsDir.string()));
		}

		// iterate through Assets dir and add all assets into their
		// respective containers based on file type

		PrefabFileExt = GetConfigData<std::string>("Prefab File Extension");
		SceneFileExt = GetConfigData<std::string>("Scene File Extension");
		AudioFileExt = GetConfigData<std::string>("Audio File Extension");
		ImageFileExt = GetConfigData<std::string>("Image File Extension");
		ShaderFileExt = GetConfigData<std::string>("Shader File Extension");
		FontFileExt = GetConfigData<std::string>("Font File Extension");
		for (const auto& file : std::filesystem::recursive_directory_iterator(assetsDir))
		{
			if (!file.is_regular_file()) { continue; }	// skip if file is a directory
			
			std::string const& currExt{ file.path().extension().string() };
			if (ImageFileExt.find(currExt) != std::string::npos)	// image
			{
				m_images.emplace(GoopUtils::ExtractPrevFolderAndFileName(file.path().string()), file.path().string());
			}
			else if (AudioFileExt.find(currExt) != std::string::npos)	// sound
			{
				m_audio.emplace(file.path().stem().string(), file.path().string());
			}
			else if (PrefabFileExt.find(currExt) != std::string::npos)	// prefab
			{
				m_prefabs.emplace(file.path().stem().string(), file.path().string());
			}
			else if (SceneFileExt.find(currExt) != std::string::npos)	// scene
			{
				m_scenes.emplace(file.path().stem().string(), file.path().string());
			}
			else if (ShaderFileExt.find(currExt) != std::string::npos)
			{
				m_shaders.emplace(file.path().filename().string(), file.path().string());
			}
			else if (FontFileExt.find(currExt) != std::string::npos)
			{
				m_fonts.emplace(file.path().filename().string(), file.path().string());
			}
			else
			{
				Debug::ErrorLogger::GetInstance().LogMessage("AssetManager: " + file.path().string() + " ignored on load");
				//throw Debug::Exception<AssetManager>(Debug::LEVEL_INFO, ErrMsg(file.path().string() + " ignored on load"));
			}
		}
		LoadImages();
		LoadSpritesheets();
		LoadFonts();

		stbi_set_flip_vertically_on_load(true);
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();
		for (auto const& curr : m_loadedSpriteData)
		{
			gEngine.CreateAnimation(curr.first, curr.second.m_slices, curr.second.m_stacks
				, curr.second.m_frames, curr.second.m_speed, 0, gEngine.textureManager.GetTextureID(curr.second.m_filePath));
		}
	}

	void AssetManager::ReloadFiles(Assets::FileType type)
	{
		std::string fileExt{};
		std::unordered_map<std::string, std::string>* ptrToMap = nullptr;
		// switch case to set the respective file extension and map
		switch (type)
		{
		case FileType::SCENE:
			m_scenes.clear();
			fileExt = GetConfigData<std::string>("Scene File Extension");
			ptrToMap = &m_scenes;
			break;
		case FileType::PREFAB:
			m_prefabs.clear();
			fileExt = GetConfigData<std::string>("Prefab File Extension");
			ptrToMap = &m_prefabs;
			break;
		case FileType::ANIMATION:
		case FileType::IMAGES:
			m_images.clear();
			fileExt = AssetManager::ImageFileExt;
			ptrToMap = &m_images;
			break;
		case FileType::AUDIO:
			m_audio.clear();
			fileExt = AssetManager::AudioFileExt;
			ptrToMap = &m_audio;
			break;
		case FileType::FONTS:
			m_fonts.clear();
			fileExt = AssetManager::FontFileExt;
			ptrToMap = &m_fonts;
		default:
			Debug::ErrorLogger::GetInstance().LogMessage("Function not coded to handle requested type");
			return;
		}

		// construct path object with relative path to project dir
		std::filesystem::path assetsDir{ GetConfigData<std::string>("Assets Dir") };
		if (!std::filesystem::exists(assetsDir))
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to open Assets Dir: " + assetsDir.string()));
		}

		// iterate through Assets dir and add all assets into their
		// respective containers based on file type
		for (const auto& file : std::filesystem::recursive_directory_iterator(assetsDir))
		{
			if (!file.is_regular_file()) { continue; }	// skip if file is a directory

			std::string const& currExt{ file.path().extension().string() };
			if (currExt == fileExt)
			{
				ptrToMap->emplace(file.path().stem().string(), file.path().string());
			}
		}
	}

	void AssetManager::ReloadAllFiles()
	{
		m_scenes.clear();
		m_prefabs.clear();
		m_images.clear();
		m_audio.clear();
		LoadFiles();
	}

	// The AssetManager class method to load JSON data from a file
	void AssetManager::LoadConfigData(const std::string& filepath)
	{
		// Create an AssetGooStream object with the given file path
		AssetGooStream ags(filepath);
		// If the AssetGooStream object is not valid, throw an exception
		if (!ags)
		{
			throw GE::Debug::Exception<AssetManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Unable to read config file"));
		}
		
		// Unload the config data from the AssetGooStream object into m_configData
		ags.Unload(m_configData);
	}

	void AssetManager::SpriteCheck()
	{
		for (auto const& entry : m_loadedSpriteData)
		{
			std::cout << "Name: " << entry.second.m_id << "\nFile: " << entry.second.m_filePath
				<< "\nSlices: " << entry.second.m_slices << "\nStacks: " << entry.second.m_stacks
				<< "\nFrames: " << entry.second.m_frames << "\n";
		}
	}

	GE::Serialization::SpriteData AssetManager::GetSpriteData(std::string key)
	{
		if (m_loadedSpriteData.find(key) == m_loadedSpriteData.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Sprite Data does not exist: " + key));
		}

		return m_loadedSpriteData[key];
	}

	void AssetManager::GetDimensions(int id, unsigned int& width, unsigned int& height)
	{
		width = m_loadedImages[id].GetWidth();
		height = m_loadedImages[id].GetHeight();
	}

	std::set<int> AssetManager::LoadDirectory(const std::string& path)
	{
		std::set<int> loaded;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
		{
			// If image file
			if (ImageFileExt.find(entry.path().extension().string()) != std::string::npos)
			{
				loaded.emplace(LoadImageW(entry.path().string()));
			}
		}
		return loaded;
	}

	bool AssetManager::AlreadyLoaded(const std::string& path)
	{
		auto pathLookup = m_loadedImagesStringLookUp.find(path);
		if (pathLookup != m_loadedImagesStringLookUp.end()) {
			return true;
		}
		return false;
	}

	bool AssetManager::AlreadyLoaded(int id)
	{
		auto pathLookup = m_loadedImagesIDLookUp.find(id);
		if (pathLookup != m_loadedImagesIDLookUp.end()) {
			return true;
		}
		return false;
	}

	unsigned AssetManager::LoadImageW(const std::string& path)
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		auto pathLookup = m_loadedImagesStringLookUp.find(GoopUtils::ExtractPrevFolderAndFileName(path));
		if (pathLookup != m_loadedImagesStringLookUp.end()) {
			// Path already exists in the map, return the existing ID or handle it as needed.
			int existingId = pathLookup->second;
			return existingId;
		}

		int width, height, channels;
		unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (img == NULL)
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load image: " + path));
		}

		ImageData imageData{ 0 , path, width, height, channels, img };
		
		unsigned TMID = gEngine.InitTexture(GE::GoopUtils::ExtractFilename(imageData.GetName()), imageData);
		imageData.SetID(TMID);

		m_loadedImages.insert(std::pair<int, ImageData>(TMID, imageData));
		m_loadedImagesStringLookUp.insert(std::pair<std::string, int>(GoopUtils::ExtractPrevFolderAndFileName(path), TMID));
		m_loadedImagesIDLookUp.insert(std::pair<int, std::string>(TMID, GoopUtils::ExtractPrevFolderAndFileName(path)));
		 
		return TMID;
	}

	void AssetManager::LoadImages()
	{
		stbi_set_flip_vertically_on_load(true);

		// Load all images in m_images
		for (std::pair<std::string, std::string> const& image : m_images)
		{
			LoadImageW(image.second);
		}
	}

	void AssetManager::FreeImages()
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		for (const auto& pair : m_loadedImages)
		{
			int id = pair.first;
			ImageData imageData = pair.second;
			if (!imageData.GetData())
			{
				// Image Data already deleted.
				continue;
			}
			// Free the loaded image data
			stbi_image_free(imageData.GetData());

			m_loadedImagesStringLookUp.erase(imageData.GetName());
			m_loadedImagesIDLookUp.erase(id);
			gEngine.DestroyTexture(gEngine.textureManager.GetTextureID(GE::GoopUtils::ExtractFilename(pair.second.GetName())));
		}

		// Clear the map of loaded images
		m_loadedImages.clear();
		m_loadedImagesStringLookUp.clear();
		m_loadedImagesIDLookUp.clear();
	}

	void AssetManager::FreeImage(const std::string& name)
	{
		std::cout << "ATTEMPTING TO FREE: " + name + "\n";
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		stbi_image_free(GetData(name).GetData());
		m_loadedImages.erase(GetData(name).GetID());
		m_loadedImagesStringLookUp.erase(GetData(name).GetName());
		m_loadedImagesIDLookUp.erase(GetData(name).GetID());
		gEngine.DestroyTexture(gEngine.textureManager.GetTextureID(name));
	}

	void AssetManager::FreeImage(int id)
	{
		stbi_image_free(GetData(id).GetData());
		m_loadedImages.erase(id);
		m_loadedImagesStringLookUp.erase(GetData(id).GetName());
		m_loadedImagesIDLookUp.erase(id);
	}

	void AssetManager::GetMapData()
	{
		std::cout << "==== [ m_images ] ====" << std::endl;
		for (const auto& x : m_images)
		{
			std::cout << x.first << " : " << x.second << std::endl;
		}

		std::cout << std::endl;

		std::cout << "==== [ m_loadedImagesStringLookUp ] ====" << std::endl;
		for (const auto& x : m_loadedImagesStringLookUp)
		{
			std::cout << x.first << " : " << x.second << std::endl;
		}

		std::cout << std::endl;

		std::cout << "==== [ m_scenes ] ====" << std::endl;
		for (const auto& x : m_scenes)
		{
			std::cout << x.first << " : " << x.second << std::endl;
		}

	}

	void AssetManager::LoadSpritesheets()
	{
		GE::Serialization::SpriteGooStream::container_type assets;
		std::string const fileName{ GetConfigData<std::string>("Sprite Config") };
		// Create a SpriteGooStream object with the given file name
		GE::Serialization::SpriteGooStream sgs{ fileName };
		// If the SpriteGooStream object is not valid, print an error message
		if (!sgs)
		{
			std::cout << "Error deserializing " << fileName << "\n";
		}
		// If unloading assets into the container was not successful, print an error message
		if (!sgs.Unload(assets))
		{
			std::cout << "Error unloading assets into container" << "\n";
		}

		// For each entry in assets, print out its details
		for (auto const& entry : assets)
		{
			SpriteData loadedData{ entry.m_id, entry.m_filePath, entry.m_slices, entry.m_stacks, entry.m_frames, entry.m_speed };
			m_loadedSpriteData.insert({ entry.m_id, loadedData });
		}
	}
}		