#include "Exception.h"
#include "Exception.h"
//#include "../ErrorLogger/ErrorLogger.h"
#include <iostream>

namespace GE
{
	namespace DebuggingTools
	{
		Exception::Exception(std::string err, std::string loc, int line)
			: m_errMsg{ err }, m_fileLoc{ loc }, m_line{ line }
		{
		}

		const char* Exception::what() const noexcept
		{
			std::cout << m_errMsg << " in " << m_fileLoc << " at " << m_line << std::endl;
			return m_errMsg.c_str();
		}
	}
}
