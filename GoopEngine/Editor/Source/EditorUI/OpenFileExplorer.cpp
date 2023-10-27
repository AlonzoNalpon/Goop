#include <pch.h>
#include <commdlg.h>

namespace GE::ChengEnLau
{
  std::string OpenFileExplorer(const char* extensionFilter)
  {
    OPENFILENAMEA fileName;
    CHAR size[260] = {};

    ZeroMemory(&fileName, sizeof(fileName));
    fileName.lStructSize = sizeof(fileName);
    fileName.hwndOwner = NULL;
    fileName.lpstrFile = size;
    fileName.nMaxFile = sizeof(size);
    fileName.lpstrFilter = extensionFilter;
    fileName.nFilterIndex = 1;
    fileName.lpstrFileTitle = NULL;
    fileName.nMaxFileTitle = 0;
    fileName.lpstrInitialDir = NULL;
    fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&fileName))
    {
      return fileName.lpstrFile;
    }

    return std::string();
  }
}