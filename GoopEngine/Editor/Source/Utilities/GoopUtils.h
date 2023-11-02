#pragma once
#include <string>

namespace GE::GoopUtils
{
  void ReloadFileData();
  std::string GetFileExtension(const std::string& filePath);
  std::string ExtractFilename(const std::string& filepath);
}