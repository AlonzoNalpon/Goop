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
#include <Serialization/Deserializer.h>
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

	std::string AssetManager::ExtractFilename(const std::string& filepath) const
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
			throw GE::Debug::Exception<AssetManager>(GE::Debug::EXCEPTION_LEVEL::LEVEL_ERROR, ErrMsg("Didn't find image data with this ID: " + id));
			//GE::Debug::ErrorLogger::GetInstance().LogError("Didn't find image data with this ID: " + id);
			//return m_loadedImagesIDLookUp[id];
		}

		return m_loadedImagesIDLookUp[id];
	}

	int AssetManager::GetID(const std::string& name)
	{
		if (m_loadedImagesStringLookUp.find(GoopUtils::ExtractPrevFolderAndFileName(name)) == m_loadedImagesStringLookUp.end())
		{
			throw GE::Debug::Exception<AssetManager>(GE::Debug::EXCEPTION_LEVEL::LEVEL_ERROR, ErrMsg("Didn't find ID with this key: " + name));
			//GE::Debug::ErrorLogger::GetInstance().LogError("Didn't find ID with this key: " + name);
			//return m_loadedImagesStringLookUp[GoopUtils::ExtractPrevFolderAndFileName(name)];
		}

		return m_loadedImagesStringLookUp[GoopUtils::ExtractPrevFolderAndFileName(name)];
	}

	std::string AssetManager::GetScene(std::string const& sceneName)
	{
		auto iter{ m_scenes.find(sceneName) };
		if (iter == m_scenes.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_ERROR, ErrMsg("Unables to get scene: " + sceneName));
		}

		return iter->second;
	}

	std::string AssetManager::GetSound(std::string const& soundName)
	{
		auto iter{ m_audio.find(soundName) };
		if (iter == m_audio.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_ERROR, ErrMsg("Unables to get audio: " + soundName));
		}
		return iter->second;
	}

	ImageData const& AssetManager::GetData(int id) const
	{
		auto iter{ m_loadedImages.find(id) };
		if (iter == m_loadedImages.end())
		{
			throw Debug::Exception<AssetManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to get data: " + id));
		}

		return iter->second;
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
		constexpr GLint FONT_SIZE{ 48 };
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
		FontFileExt = GetConfigData<std::string>("Font File Extension"); 
		ShaderFileExt = GetConfigData<std::string>("Shader File Extension");
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
			try
			{
				gEngine.CreateAnimation(curr.first, curr.second.m_slices, curr.second.m_stacks
					, curr.second.m_frames, curr.second.m_speed, curr.second.m_flags, gEngine.textureManager.GetTextureID(curr.second.m_filePath));
			}
			catch (GE::Debug::IExceptionBase& e)
			{
				e.LogSource();
			}
		}
	}

	void AssetManager::ReloadFiles(Assets::AssetType type)
	{
		std::string fileExt{};
		std::unordered_map<std::string, std::string>* ptrToMap = nullptr;
		// switch case to set the respective file extension and map
		switch (type)
		{
		case AssetType::SCENE:
			m_scenes.clear();
			fileExt = GetConfigData<std::string>("Scene File Extension");
			ptrToMap = &m_scenes;
			break;
		case AssetType::PREFAB:
			m_prefabs.clear();
			fileExt = GetConfigData<std::string>("Prefab File Extension");
			ptrToMap = &m_prefabs;
			break;
		case AssetType::ANIMATION:
		case AssetType::IMAGES:
			FreeImages();
			m_images.clear();
			fileExt = AssetManager::ImageFileExt;
			ptrToMap = &m_images;
			break;
		case AssetType::AUDIO:
			m_audio.clear();
			fileExt = AssetManager::AudioFileExt;
			ptrToMap = &m_audio;
			break;
		case AssetType::FONTS:
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
			if (currExt.find(fileExt) != std::string::npos)
			{
				ptrToMap->emplace(file.path().stem().string(), file.path().string());
			}
		}

		if (type == AssetType::IMAGES || type == AssetType::ANIMATION)
		{
			ReloadAllFiles();
		}
		else if (type == AssetType::FONTS)
		{
			LoadFonts();
		}
	}

	void AssetManager::ReloadAllFiles()
	{
		FreeImages();
		m_images.clear();
		Graphics::GraphicsEngine::GetInstance().FreeTexturesAndFonts();
		m_audio.clear();
		m_scenes.clear();
		m_fonts.clear();
		m_shaders.clear();
		LoadFiles();
	}

	void AssetManager::ClearConfigData()
	{
		m_configData.clear();
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

#ifndef IMGUI_DISABLE
	void AssetManager::HandleEvent(Events::Event* event)
	{
		switch (event->GetCategory())
		{
		case Events::EVENT_TYPE::NEW_SCENE:
		{
			std::string const& name{ static_cast<Events::NewSceneEvent*>(event)->m_sceneName };
			// get path
			std::string path{ GetConfigData<std::string>("Assets Dir") + "Scenes/" + name + SceneFileExt };

			// create the file
			std::ofstream ofs{ path };
			if (!ofs)
			{
				throw Debug::Exception<AssetManager>(Debug::LEVEL_ERROR, ErrMsg("Unable to create " + path));
			}
			ofs.close();

			// insert into map
			m_scenes[name] = std::move(path);
			break;
		}

		// clear from respective map
		case Events::EVENT_TYPE::DELETE_ASSET:
		{
			Events::DeleteAssetEvent* assetEvent{ static_cast<Events::DeleteAssetEvent*>(event) };
			switch (assetEvent->m_type)
			{
			case AssetType::SCENE:
				m_scenes.erase(assetEvent->m_name);
				break;
			case AssetType::ANIMATION:
			case AssetType::IMAGES:
				FreeImage(assetEvent->m_name);
				m_images.erase(assetEvent->m_name);
				
				break;
			case AssetType::AUDIO:
				m_audio.erase(assetEvent->m_name);
				break;
			case AssetType::FONTS:
				m_fonts.erase(assetEvent->m_name);
				break;			
			case AssetType::SHADERS:
				m_shaders.erase(assetEvent->m_name);
				break;
			}
			break;
		}
		}
	}
#endif

	void AssetManager::SpriteCheck()
	{
		for (auto const& entry : m_loadedSpriteData)
		{
			Debug::ErrorLogger::GetInstance().LogMessage("Name: " + entry.second.m_id);
			Debug::ErrorLogger::GetInstance().LogMessage("File: " + entry.second.m_filePath);
			Debug::ErrorLogger::GetInstance().LogMessage("Slices: " + entry.second.m_slices);
			Debug::ErrorLogger::GetInstance().LogMessage("Stacks: " + entry.second.m_stacks);
			Debug::ErrorLogger::GetInstance().LogMessage("Frames: " + entry.second.m_frames);
		}
	}

	GE::Serialization::SpriteData AssetManager::GetSpriteData(std::string const& key)
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

	bool AssetManager::AlreadyLoaded(const std::string& path) const
	{
		auto pathLookup = m_loadedImagesStringLookUp.find(path);
		if (pathLookup != m_loadedImagesStringLookUp.end()) {
			return true;
		}
		return false;
	}

	bool AssetManager::AlreadyLoaded(int id) const
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
		stbi_image_free(img);
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
			try
			{
				LoadImageW(image.second);
			}
			catch (GE::Debug::IExceptionBase& e)
			{
				e.LogSource();
			}
		}
	}

	void AssetManager::FreeImages()
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		for (const auto& [imgid, imgdata] : m_loadedImages)
		{
			m_loadedImagesStringLookUp.erase(imgdata.GetName());
			m_loadedImagesIDLookUp.erase(imgid);
			gEngine.DestroyTexture(imgid);
		}
		gEngine.FreeSpriteAnimations();

		// Clear the map of loaded images
		m_loadedImages.clear();
		m_loadedImagesStringLookUp.clear();
		m_loadedImagesIDLookUp.clear();
	}

	void AssetManager::FreeImage(const std::string& name)
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		try
		{
			m_loadedImages.erase(GetData(name).GetID());
			m_loadedImagesStringLookUp.erase(GetData(name).GetName());
			m_loadedImagesIDLookUp.erase(GetData(name).GetID());
			gEngine.DestroyTexture(gEngine.textureManager.GetTextureID(name));
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
	}

	void AssetManager::FreeImage(int id)
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();

		try
		{
			m_loadedImages.erase(id);
			m_loadedImagesStringLookUp.erase(GetData(id).GetName());
			m_loadedImagesIDLookUp.erase(id);
			gEngine.DestroyTexture(id);
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
	}

	void AssetManager::GetMapData()
	{
		Debug::ErrorLogger::GetInstance().LogMessage("=== [ m_images ] ===");
		for (const auto& x : m_images)
		{
			Debug::ErrorLogger::GetInstance().LogMessage(x.first + " : " + x.second);
		}

		Debug::ErrorLogger::GetInstance().LogMessage("");
		Debug::ErrorLogger::GetInstance().LogMessage("==== [ m_loadedImages ] ====");

		for (const auto& x : m_loadedImages)
		{
			Debug::ErrorLogger::GetInstance().LogMessage(x.second.GetName());
		}

		Debug::ErrorLogger::GetInstance().LogMessage("");
		Debug::ErrorLogger::GetInstance().LogMessage("==== [ m_loadedImagesStringLookUp ] ====");

		for (const auto& x : m_loadedImagesStringLookUp)
		{
			Debug::ErrorLogger::GetInstance().LogMessage(x.first);
		}

		Debug::ErrorLogger::GetInstance().LogMessage("");
		Debug::ErrorLogger::GetInstance().LogMessage("==== [ m_scenes ] ====");

		for (const auto& x : m_scenes)
		{
			Debug::ErrorLogger::GetInstance().LogMessage(x.first + " : " + x.second);
		}

	}

	void AssetManager::LoadSpritesheets()
	{
		std::string const fileName{ GetConfigData<std::string>("Sprite Config") };
		auto assets{ Deserializer::DeserializeSpriteSheetData(fileName) };

		// For each entry in assets, print out its details
		for (auto& entry : assets)
		{
			std::string id{ entry.m_id };
			m_loadedSpriteData.emplace(std::make_pair(std::move(id), std::move(entry)));
		}
	}
}		