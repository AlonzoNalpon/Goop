#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include "../Singleton/Singleton.h"


namespace GE
{
  namespace AssetManager
  {
    class IDGenerator
    {
    public:
      int GenerateID();
      void FreeID(int id);

    private:
      int m_next_id = 0;
      std::unordered_set<int> m_recycled_id;
    };

    class ImageData
    {
    public:
      ImageData() : m_id(0), m_name(""), m_width(0), m_height(0), m_channels(0), m_data(nullptr) {};
      ImageData(int id, std::string name, int width, int height, int channels, unsigned char* data) :
        m_id(id), m_name(name), m_width(width), m_height(height), m_channels(channels), m_data(data) {};

      void SetName(const std::string& name);
      void SetInfo(int width, int height, int channels, unsigned char* data);

    private:
      int m_id;
      std::string m_name;
      int m_width;
      int m_height;
      int m_channels;
      unsigned char* m_data;
    };

    class AssetManager : public Singleton<AssetManager>
    {
    public:

      void LoadDirectory(const std::string& directory);
      int LoadImage(const std::string& path);
      const std::string& GetName(int id);
      int GetID(const std::string& name);
      ImageData GetData(int id);
      ImageData GetData(const std::string& name);
      void LoadDeseralizedData();

    private:
      IDGenerator m_generator;
      std::map<int, ImageData> m_loadedImages;
      std::map<std::string, int> m_loadedImages_string_LU; //Lookup table for getting ID with filepath.
      std::map<int, std::string> m_loadedImages_ID_LU; //Lookup table for getting filepath with id.

    };
  }
}
#endif