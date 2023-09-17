/*!******************************************************************
\file   ErrorLogger.cpp 
\author w.chinkitbryam\@digipen.edu
\date   17 September 2023
\brief  
  Defines non template functions for logging. Constructor opens
	ile for spdlog to write to.

	Destructor will then rename the file to current time of call.
	his is so the log file is easier to find as the time is closer
	o which one would be looking for the logs.
********************************************************************/

#include "ErrorLogger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace GE::Debug;

ErrorLogger::ErrorLogger()
{
	// Get curret system time
	std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
	std::time_t currTime_t = std::chrono::system_clock::to_time_t(currTime);

	// Convert to format usable by put_time()
	std::tm* localTime = std::localtime(&currTime_t);

	std::stringstream ss;
	// not sure if file path should be changable by config file
	ss << std::put_time(localTime, "Logs/Log_%Y.%m.%d-%H.%M.%S.txt");

	// Create sinks, where loggers will dump their info into
	m_logDump = std::make_shared<spdlog::sinks::basic_file_sink_mt>(ss.str(), true);
	m_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	// Default file logger 
	LoggerPtr defLogger = std::make_shared<spdlog::logger>("", m_logDump);
	m_fileLoggers[""] = defLogger;

	// Default stream logger
	defLogger = std::make_shared<spdlog::logger>("", m_logger);
	m_streamLoggers[""] = defLogger;
}

ErrorLogger::~ErrorLogger()
{
	// Flush logs
	for (auto lp : m_fileLoggers)
	{
		lp.second->flush();
	}
	for (auto lp : m_streamLoggers)
	{
		lp.second->flush();
	}

	// Rename the created file with the current time of end of application
	// This is so its easier to find the log file as it matches the time
	// someone usually looks for in recent logs
	
	// Get curret system time
	std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
	std::time_t currTime_t = std::chrono::system_clock::to_time_t(currTime);

	// Convert to format usable by put_time()
	std::tm* localTime = std::localtime(&currTime_t);

	std::stringstream ss;
	// not sure if file path should be changable by config file
	ss << std::put_time(localTime, "Logs/Log_%Y.%m.%d-%H.%M.%S.txt");

	std::rename(m_logDump->filename().c_str(), ss.str().c_str());
}

std::string ErrorLogger::LogMessage(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_fileLoggers[""]->info(msg);
  }
  m_streamLoggers[""]->info(msg);

	return msg;
}

std::string ErrorLogger::LogWarning(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_fileLoggers[""]->warn(msg);
  }
  m_streamLoggers[""]->warn(msg);

	return msg;
}

std::string ErrorLogger::LogError(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_fileLoggers[""]->error(msg);
  }
  m_streamLoggers[""]->error(msg);

	return msg;
}

std::string ErrorLogger::LogCritical(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_fileLoggers[""]->critical(msg);
  }
  m_streamLoggers[""]->critical(msg);

	return msg;
}