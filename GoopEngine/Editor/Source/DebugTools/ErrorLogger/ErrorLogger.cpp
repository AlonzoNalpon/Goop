/*!*********************************************************************
\file   ErrorLogger.cpp 
\author w.chinkitbryam\@digipen.edu
\date   17 September 2023
\brief  
  Defines non template functions for logging. Constructor opens
	ile for spdlog to write to.

	Destructor will then rename the file to current time of call.
	his is so the log file is easier to find as the time is closer
	o which one would be looking for the logs.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ErrorLogger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace GE::Debug;

ErrorLogger::ErrorLogger() : m_wroteToFile{ false }
{
	// Get curret system time
	std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
	std::time_t currTime_t = std::chrono::system_clock::to_time_t(currTime);

	// Convert to format usable by put_time()
	std::tm* localTime = std::localtime(&currTime_t);

	std::stringstream ss;
	// not sure if file path should be changable by config file
	ss << std::put_time(localTime, "Logs/Log_%Y.%m.%d-%H.%M.%S.txt");
	m_fileName = ss.str();

	auto filesink = std::make_shared<spdlog::sinks::basic_file_sink_st>(m_fileName, true);

	// Default logger will log only to ostream
	m_logger = std::make_unique<spdlog::logger>("");
	// File logger will log into ostream and file
	m_fileLogger = std::make_unique<spdlog::logger>("", filesink);
}

ErrorLogger::~ErrorLogger()
{
	// Flush logs
	m_logger->flush();
	m_logger.reset();

	// If did not write to file there will be an empty log file. Remove it
	if (!m_wroteToFile)
	{
		std::remove(m_fileName.c_str());
	}
	else
	{
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

		std::rename(m_fileName.c_str(), ss.str().c_str());
	}
}

void GE::Debug::ErrorLogger::AddSink(spdlog::sink_ptr sink)
{
	m_logger->sinks().push_back(sink);
}

void GE::Debug::ErrorLogger::AddFileSink(spdlog::sink_ptr sink)
{
	m_fileLogger->sinks().push_back(sink);
}

void ErrorLogger::SuppressLogMessages(bool flag)
{
	if (flag)
	{
		m_logger->set_level(spdlog::level::warn);
		m_fileLogger->set_level(spdlog::level::warn);
	}
	else
	{
		m_logger->set_level(spdlog::level::trace);
		m_fileLogger->set_level(spdlog::level::trace);
	}
}

std::string ErrorLogger::LogMessage(std::string msg, bool logToFile)
{
	// Don't log messages
  if (logToFile)
  {
		m_wroteToFile = true;
		m_fileLogger->info(msg);
  }
	else
	{
		m_logger->info(msg);
	}

	return msg;
}

std::string ErrorLogger::LogWarning(std::string msg, bool logToFile)
{
	if (logToFile)
	{
		m_wroteToFile = true;
		m_fileLogger->warn(msg);
	}
	else
	{
		m_logger->warn(msg);
	}

	return msg;
}

std::string ErrorLogger::LogError(std::string msg, bool logToFile)
{
	if (logToFile)
	{
		m_wroteToFile = true;
		m_fileLogger->error(msg);
	}
	else
	{
		m_logger->error(msg);
	}

	return msg;
}

std::string ErrorLogger::LogCritical(std::string msg, bool logToFile)
{
	if (logToFile)
	{
		m_wroteToFile = true;
		m_fileLogger->critical(msg);
	}
	else
	{
		m_logger->critical(msg);
	}

	return msg;
}