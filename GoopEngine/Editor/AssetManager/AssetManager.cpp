/*!*********************************************************************
\file   AssetManager.cpp
\author loh.j@digipen.edu
\date   13-September-2023
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
#include <AssetManager/AssetManager.h>
#include <filesystem>
#include <Graphics/GraphicsEngine.h>
#include "../Serialization/AssetGooStream.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#define ASSET_MANAGER_DEBUG
using namespace GE::Serialization;

namespace GE::AssetManager
{
	AssetManager::~AssetManager()
	{
		FreeImages();
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
		if (m_loadedImagesStringLookUp.find(name) == m_loadedImagesStringLookUp.end())
		{
			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND ID WITH KEY: " << name << "\n";
			#endif
			return m_loadedImagesStringLookUp[name];
		}

		return m_loadedImagesStringLookUp[name];

	}
	ImageData const& AssetManager::GetData(int id)
	{
		if (m_loadedImages.find(id) == m_loadedImages.end())
		{
			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND DATA WITH ID: " << id << "\n";
			#endif
			//return ImageData::Null();
			throw;
		}

		return m_loadedImages[id];
	}
	ImageData const& AssetManager::GetData(const std::string& name)
	{
		if (m_loadedImages.find(GetID(name)) == m_loadedImages.end())
		{
			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND DATA WITH ID: " << id << "\n";
			#endif
			//return ImageData::Null();
			throw;
		}

		return m_loadedImages[GetID(name)];
	}

	void AssetManager::LoadFiles()
	{
		stbi_set_flip_vertically_on_load(true);
		LoadJSONData(GetConfigData<std::string>("Stills").value(), GE::AssetManager::IMAGES);
		LoadJSONData(GetConfigData<std::string>("Animation Sprites").value(), GE::AssetManager::ANIMATION);
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();
		for (auto const& curr : m_loadedSpriteData)
		{
			gEngine.CreateAnimation(curr.first, curr.second.m_slices, curr.second.m_stacks
				, curr.second.m_frames, curr.second.m_speed, 0, gEngine.textureManager.GetTextureID(curr.second.m_filePath));
		}
	}

	// The AssetManager class method to load JSON data from a file
	void AssetManager::LoadJSONData(const std::string& filepath, int flag)
	{
		auto& gEngine = Graphics::GraphicsEngine::GetInstance();
		// If the flag is set to IMAGES or CONFIG
		if (flag == IMAGES || flag == CONFIG)
		{
			// Create an AssetGooStream object with the given file path
			AssetGooStream ags(filepath);
			// If the AssetGooStream object is not valid, throw an exception
			if (!ags)
			{
				//throw exception
			}

			// If the flag is set to IMAGES
			if (flag == IMAGES)
			{
				// Unload the file path from the AssetGooStream object into m_filePath
				ags.Unload(m_filePath);
				// If the unloading was not successful, throw an exception
				if (!ags.Success())
				{
					//throw exception
				}

				// For each entry in m_filePath, load the image from the ASSETS_DIR and the entry's second value
				for (std::pair<std::string, std::string> const& entry : m_filePath)
				{
					int id = LoadImageW(entry.second);

					gEngine.InitTexture(entry.first, GetData(id));
				}
			}
			// If the flag is set to CONFIG
			else if (flag == CONFIG)
			{
				// Unload the config data from the AssetGooStream object into m_configData
				ags.Unload(m_configData);

#ifdef ASSET_MANAGER_DEBUG
				// For each entry in m_configData, print out the key-value pair if ASSET_MANAGER_DEBUG is defined
				for (std::pair<std::string, std::string> const& entry : m_configData)
				{
					std::cout << "\"" << entry.first << "\" : \"" << entry.second << "\"" << std::endl;
				}
#endif    
			}
		}
		// If the flag is set to ANIMATION
		if (flag == ANIMATION)
		{
			// Create a container for assets
			GE::Serialization::SpriteGooStream::container_type assets;
			std::string const fileName{ GetConfigData<std::string>("Animation Sprites").value() };
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

			std::cout << "Deserialized " << fileName << ":\n";

			// For each entry in assets, print out its details
			for (auto const& entry : assets)
			{
				SpriteData loadedData{ entry.m_id, entry.m_filePath, entry.m_slices, entry.m_stacks, entry.m_frames, entry.m_speed };
				m_loadedSpriteData.insert({ entry.m_id, loadedData });
			}
		}
		#ifdef ASSET_MANAGER_DEBUG
		printf("Loading success\n");
		#endif
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
			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "DIDN'T FIND SPRITE DATA WITH: " << key << "\n";
			#endif
			return m_loadedSpriteData[key];

		#ifdef ASSET_MANAGER_DEBUG
		std::cout << "SPRITE DATA RETRIEVED: " << m_loadedSpriteData[key] << "\n";
		#endif

		return m_loadedSpriteData[key];
	}

	void AssetManager::GetDimensions(int id, unsigned int& width, unsigned int& height)
	{
		width = m_loadedImages[id].GetWidth();
		height = m_loadedImages[id].GetHeight();
	}

	void AssetManager::LoadDirectory(const std::string& path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			LoadImageW(entry.path().string());
		}
	}

	int AssetManager::LoadImageW(const std::string& path)
	{
		int width, height, channels;
		int id = m_generator.GenerateID();

		unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (img == NULL)
		{
			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "Error encountered while loading: " << path.c_str() << "\n";
			#endif
		}
		else {
			#ifdef ASSET_MANAGER_DEBUG
			printf("Successfully loaded %s. (ID: %d Width: %d px Height: %d px Channel: %d)\n", path.c_str(), id, width, height, channels);
			#endif
		}

		ImageData imageData{id, path, width, height, channels, img};
		m_loadedImages.insert(std::pair<int, ImageData>(id, imageData));
		m_loadedImagesStringLookUp.insert(std::pair<std::string, int>(path, id));
		m_loadedImagesIDLookUp.insert(std::pair<int, std::string>(id, path));

		return id;
	}

	std::vector<std::pair<std::string, std::string>> AssetManager::MOCK_Deserialize()
	{
		std::map<std::string, std::string> data;
		data["Character Sprite"] = "Assets/Knight.png";
		data["Monster 1"] = "Assets/Buta PIG.png";
		data["Monster 2"] = "Assets/Green Girl.png";
		data["Slash VFX"] = "Assets/redGirl.png";
		data["FONT"] = "Assets/Blue Tiles.png";

		// Create a vector to hold the key-value pairs
		std::vector<std::pair<std::string, std::string>> result;

		// Iterate through the map and add key-value pairs to the vector
		for (const auto& pair : data)
		{
			result.push_back(pair);
		}

		return result;
	}

	void AssetManager::LoadDeserializedData()
	{
		stbi_set_flip_vertically_on_load(true);
		std::vector<std::pair<std::string, std::string>> deserialized_data = MOCK_Deserialize();
			
		for (const std::pair<const std::string, std::string>& value : deserialized_data) {
			std::string mapValue = value.second;
			LoadImageW(mapValue);
		}
	}

	void AssetManager::FreeImages()
	{
		for (const auto& pair : m_loadedImages)
		{
				
			int id = pair.first;
			ImageData imageData = pair.second;
			if (!imageData.GetData())
			{
				continue;
			}
			#ifdef ASSET_MANAGER_DEBUG
			printf("Freeing Image: %s...\n", imageData.GetName().c_str());
			#endif
			// Free the loaded image data
			stbi_image_free(imageData.GetData());

			m_loadedImagesStringLookUp.erase(imageData.GetName());
			m_loadedImagesIDLookUp.erase(id);
		}

		// Clear the map of loaded images
		m_loadedImages.clear();
		m_loadedImagesStringLookUp.clear();
		m_loadedImagesIDLookUp.clear();

		#ifdef ASSET_MANAGER_DEBUG
		printf("Successfully cleared data.");
		#endif
	}

	void AssetManager::FreeImage(const std::string& name)
	{
		stbi_image_free(GetData(name).GetData());
		m_loadedImages.erase(GetData(name).GetID());
		m_loadedImagesStringLookUp.erase(GetData(name).GetName());
		m_loadedImagesIDLookUp.erase(GetData(name).GetID());
	}
}
