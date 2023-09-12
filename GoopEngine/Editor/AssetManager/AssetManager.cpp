#include <../AssetManager/AssetManager.h>
#include <filesystem>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#define ASSET_M_DEBUG

namespace Asset_M {

	std::vector<ImageData> loadedImages;

	void LoadImage(const std::string& path, const std::string& filename)
	{
		#ifdef ASSET_M_DEBUG
		printf("Loading %s... ", filename.c_str()); 
		#endif
		std::string full_path = path + filename;
		int width, height, channels;

		unsigned char* img = stbi_load(full_path.c_str(), &width, &height, &channels, 0);
		
		if (img == NULL)
		{
			printf("Error in loading image!!\n");
		}
		else {
			#ifdef ASSET_M_DEBUG
			printf("Successfully loaded the image. (Width: %d px Height: %d px Channel: %d)\n", width, height, channels);
			#endif
		}

		ImageData imageData;
		imageData.name = filename;
		imageData.width = width;
		imageData.height = height;
		imageData.channels = channels;
		imageData.data = img;

		loadedImages.push_back(imageData);
	}

	void LoadChecker()
	{
		for (size_t i = 0; i < loadedImages.size(); ++i)
		{
			#ifdef ASSET_M_DEBUG
			printf("Loaded asset's name: %s\n", loadedImages[i].name.c_str());
			#endif
		}
	}

	void FreeImage()
	{
		for (size_t i = 0; i < loadedImages.size(); ++i)
		{
			stbi_image_free(loadedImages[i].data);

			#ifdef ASSET_M_DEBUG
			printf("%s unloaded.\n", loadedImages[i].name.c_str());
			#endif
		}
		loadedImages.clear();
	}

	void FilenameHarvester()
	{
		std::string path = "Assets/";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::filesystem::path p(entry.path());
			LoadImage(path, p.filename().string());
		}
	}
}
