/*!******************************************************************
\file   Exception.cpp 
\author w.chinkitbryam\@digipen.edu
\date   17 September 2023
\brief  
  Custom exception class with spdlog integration.

  Defines non template functions for exceptions.
********************************************************************/

#include "Exception.h"
#include "../ErrorLogger/ErrorLogger.h"
#include <sstream>

using namespace GE::Debug;

template <typename T>
Exception<T>::Exception(EXCEPTION_LEVEL lvl, std::string err, std::string loc, int line)
	: m_errMsg{ err }, m_loc{ loc }, m_line{ line }, m_lvl{ lvl }, m_el{ ErrorLogger::GetInstance() }
{
	m_errMsg = ConstructMessage();
}

template <typename T>
std::string Exception<T>::ConstructMessage()
{
  std::stringstream ss;
	ss << m_loc << "(line " << m_line << ") " << m_errMsg;

  return ss.str();
}

template <typename T>
std::string Exception<T>::Log(bool logToFile)
{
  switch (m_lvl)
  {
  case Exception::LEVEL_WARN:
    return m_el.LogWarning(m_errMsg, logToFile);
    break;
  case Exception::LEVEL_ERROR:
    return m_el.LogError(m_errMsg, logToFile);
    break;
  case Exception::LEVEL_CRITICAL:
    return m_el.LogCritical(m_errMsg, logToFile);
    break;
  default: // Default will assume throw is just info
  case Exception::LEVEL_INFO:
    return m_el.LogMessage(m_errMsg, logToFile);
    break;
  }
}
