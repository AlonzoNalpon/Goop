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

		const std::string& AssetManager::GetName(int id)
		{
			printf("Get: %d and received: %s\n", id, m_loadedImages_ID_LU[id].c_str());
			return m_loadedImages_ID_LU[id];
		}
		int AssetManager::GetID(const std::string& name)
		{
			printf("Get: %s and received: %d\n", name.c_str(), m_loadedImages_string_LU[name]);
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
	}
}
//std::vector <std::pair<std::string, std::string>> vec = Deserialize(file);
