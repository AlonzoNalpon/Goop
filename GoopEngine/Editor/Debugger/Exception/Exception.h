#pragma once
#include <exception>
#include <string>

#define ERRLG_FILE __FILE__
#define ERRLG_FUNC __FUNCTION__
#define ERRLG_LINE __LINE__

namespace GE
{
  namespace DebuggingTools
  {
    
    class Exception : public std::exception
    {
    private:
      std::string m_errMsg{"Unknown Error"};
      std::string m_fileLoc{"Unknown Location"};
      int m_line;
    public:
      // Must log error with string
      Exception() = delete;
      Exception(std::string err, std::string loc, int line);
      // Copy ctor & copy assignment are default
      const char* what() const noexcept;
    };
  }
}
