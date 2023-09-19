/*!*********************************************************************
\file   AssetManager.cpp
\author loh.j@digipen.edu
\date   13-September-2023
\brief
	Asset Manager capable of:
		- LoadDirectory
				Loading all files located in directory

		- LoadImage
				Loading specific image from path

		- GetData [WIP]
				Get image data for alonzo.

		- LoadDeserializedData [WIP]
				Read filepath from deserialized data and load files.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <../AssetManager/AssetManager.h>
#include <filesystem>
#include <iostream>
#include "../Serialization/AssetGooStream.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define ASSET_MANAGER_DEBUG
using namespace GE::Serialization;

namespace GE
{
	namespace AssetManager
	{
		std::string const ASSETS_DIR{ "../Assets/AssetsToLoadTest/" };

		IDGenerator::~IDGenerator()
		{
			m_recycledID.clear();
		}

		int IDGenerator::GenerateID()
		{
			if (!m_recycledID.empty())
			{
				int recycledID = *m_recycledID.begin();
				m_recycledID.erase(recycledID);
				return recycledID;
			}
			return m_nextID++;
		}
		void IDGenerator::FreeID(int id)
		{
			m_recycledID.insert(id);
		}

		void ImageData::SetName(const std::string& name)
		{
			this->m_name = name;
		}

		void ImageData::SetInfo(int width, int height, int channels, unsigned char* data)
		{
			this->m_width = width;
			this->m_height = height;
			this->m_channels = channels;
			this->m_data = data;
		}

		unsigned char* ImageData::GetData()
		{
			return this->m_data;
		}

		unsigned int ImageData::GetWidth()
		{
			return this->m_width;
		}

		unsigned int ImageData::GetHeight()
		{
			return this->m_height;
		}

		std::string ImageData::GetName()
		{
			return this->m_name;
		}

		int ImageData::GetID()
		{
			return this->m_id;
		}

		AssetManager::~AssetManager()
		{
			FreeImages();
		}

		const std::string& AssetManager::GetName(int id)
		{
			return m_loadedImagesIDLookUp[id];
		}
		int AssetManager::GetID(const std::string& name)
		{
			return m_loadedImagesStringLookUp[name];

		}
		ImageData AssetManager::GetData(int id)
		{
			return m_loadedImages[id];
		}
		ImageData AssetManager::GetData(const std::string& name)
		{
			return m_loadedImages[GetID(name)];
		}

		void AssetManager::LoadJSONData(const std::string& filepath, int flag)
		{
			AssetGooStream ags(filepath);
			if (!ags)
			{
				//throw exception
			}

			if (flag == IMAGES)
			{
				ags.Unload(m_filePath);
				if (!ags.Success())
				{
					//throw exception
				}

				for (std::pair<std::string, std::string> const& entry : m_filePath)
				{
					LoadImage(ASSETS_DIR + entry.second);
				}
			}
			if (flag == CONFIG)
			{
				ags.Unload(m_configData);

				for (std::pair<std::string, std::string> const& entry : m_configData)
				{
					std::cout << "\"" << entry.first << "\" : \"" << entry.second << "\"" << std::endl;
				}
			}
			#ifdef ASSET_MANAGER_DEBUG
			printf("Loading success\n");
			#endif
		}

		int AssetManager::GetConfigData(const std::string& key)
		{
			if (m_configData.find(key) == m_configData.end())
			{
				return 0;
			}

			#ifdef ASSET_MANAGER_DEBUG
			std::cout << "CONFIG DATA RETRIEVED: " << m_configData[key] << "\n";
			#endif

			return std::stoi(m_configData[key]);
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
				LoadImage(entry.path().string());
			}
		}

		int AssetManager::LoadImage(const std::string& path)
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
			std::vector<std::pair<std::string, std::string>> deserialized_data = MOCK_Deserialize();
			
			for (const std::pair<const std::string, std::string>& value : deserialized_data) {
				std::string mapValue = value.second;
				LoadImage(mapValue);
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
}
