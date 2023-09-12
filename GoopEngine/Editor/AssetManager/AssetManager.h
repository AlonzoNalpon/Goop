#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include <string>

namespace Asset_M {
	void FilenameHarvester();
    void LoadImage(const std::string& path, const std::string& filename);
    void LoadChecker();
    void FreeImage();

}struct ImageData {
    std::string name;
    int width;
    int height;
    int channels;
    unsigned char* data;

    ImageData() : width(0), height(0), channels(0), data(nullptr) {}
};
#endif