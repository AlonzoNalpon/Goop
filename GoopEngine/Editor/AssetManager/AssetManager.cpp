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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define ASSET_M_DEBUG

namespace GE
{
	namespace AssetManager
	{
		int IDGenerator::GenerateID()
		{
			if (!m_recycled_id.empty())
			{
				int recycledID = *m_recycled_id.begin();
				m_recycled_id.erase(recycledID);
				return recycledID;
			}
			return m_next_id++;
		}
		void IDGenerator::FreeID(int id)
		{
			m_recycled_id.insert(id);
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

		unsigned int ImageData::GetChannels()
		{
			return this->m_channels;
		}

		std::string ImageData::GetName()
		{
			return this->m_name;
		}

		int ImageData::GetID()
		{
			return this->m_id;
		}

		const std::string& AssetManager::GetName(int id)
		{
			return m_loadedImages_ID_LU[id];
		}
		int AssetManager::GetID(const std::string& name)
		{
			return m_loadedImages_string_LU[name];

		}
		ImageData AssetManager::GetData(int id)
		{
			return m_loadedImages[id];
		}
		ImageData AssetManager::GetData(const std::string& name)
		{
			return m_loadedImages[GetID(name)];
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
				printf("Error in loading image!!\n");
			}
			else {
#ifdef ASSET_M_DEBUG
				printf("Successfully loaded %s. (ID: %d Width: %d px Height: %d px Channel: %d)\n", path.c_str(), id, width, height, channels);
#endif
			}

			ImageData imageData{id, path, width, height, channels, img};
			m_loadedImages.insert(std::pair<int, ImageData>(id, imageData));
			m_loadedImages_string_LU.insert(std::pair<std::string, int>(path, id));
			m_loadedImages_ID_LU.insert(std::pair<int, std::string>(id, path));

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
				printf("Freeing Image: %s...\n", imageData.GetName().c_str());
				// Free the loaded image data
				stbi_image_free(imageData.GetData());

				m_loadedImages_string_LU.erase(imageData.GetName());
				m_loadedImages_ID_LU.erase(id);
			}

			// Clear the map of loaded images
			m_loadedImages.clear();
			m_loadedImages_string_LU.clear();
			m_loadedImages_ID_LU.clear();
			printf("Successfully cleared data.");
		}

		void AssetManager::FreeImage(const std::string& name)
		{
			stbi_image_free(GetData(name).GetData());
			m_loadedImages.erase(GetData(name).GetID());
			m_loadedImages_string_LU.erase(GetData(name).GetName());
			m_loadedImages_ID_LU.erase(GetData(name).GetID());
		}
	}
}
