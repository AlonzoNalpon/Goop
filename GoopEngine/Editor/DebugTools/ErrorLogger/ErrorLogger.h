/*!******************************************************************
\file   ErrorLogger.h 
\author w.chinkitbryam\@digipen.edu
\date   17 September 2023
\brief  
  Singleton error logger holding spdlog objects for logging.
  The class is not static as it is required to hold data ie.
  streams for output and to dump all logs to a file by default
********************************************************************/
#pragma once
#include <spdlog/spdlog.h>
#include "../../Singleton/Singleton.h"
#include <memory>
#include <map>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace spdlog;

namespace GE
{
  namespace Debug
  {
    class ErrorLogger : public Singleton<ErrorLogger>
    {
    private:
      // Alias
      using LoggerPtr = std::shared_ptr<spdlog::logger>;

      // File sink
      std::shared_ptr<spdlog::sinks::basic_file_sink_st> m_logDump;
      // Ostream sink
      std::shared_ptr<spdlog::sinks::stdout_color_sink_st> m_logger;

      // Map of loggers pointing to file sink
      std::map<std::string, LoggerPtr> m_fileLoggers;
      // Map of loggers pointing to ostream sink
      std::map<std::string, LoggerPtr> m_streamLoggers;

      bool m_suppressLogWarning;
      bool m_wroteToFile;

      /*!******************************************************************
      \brief 
        Returns a shared pointer to a logger with a file sink of a type
      \return 
        Pointer to a logger
      ********************************************************************/
      template <typename T>
      LoggerPtr GetFileLogger();

      /*!******************************************************************
      \brief
        Returns a shared pointer to a logger with a ostream sink of a type
      \return
        Pointer to a logger
      ********************************************************************/
      template <typename T>
      LoggerPtr GetStreamLogger();

    public:
      /*!******************************************************************
      \brief 
        Setups all the intial sink and loggers to different streams.
      ********************************************************************/
      ErrorLogger();
      /*!******************************************************************
      \brief
        Flushes all loggers into their streams and renames output file
        to match the current time of exit.
      ********************************************************************/
      ~ErrorLogger();

      void SuppressLogMessages(bool flag);

      /*!******************************************************************
      \brief 
        Logs a message with Logger name being T, with custom input message.
        The log level of this function is info.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      template <typename T>
      std::string LogMessage(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a nameless message with custom input message.
        The log level of this function is info.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      std::string LogMessage(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a message with Logger name being T, with custom input message.
        The log level of this function is warning.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      template <typename T>
      std::string LogWarning(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a nameless message with custom input message.
        The log level of this function is warning.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      std::string LogWarning(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a message with Logger name being T, with custom input message.
        The log level of this function is error.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      template <typename T>
      std::string LogError(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a nameless message with custom input message.
        The log level of this function is error.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      std::string LogError(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a message with Logger name being T, with custom input message.
        The log level of this function is critical.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      template <typename T>
      std::string LogCritical(std::string msg, bool logToFile = true);

      /*!******************************************************************
      \brief
        Logs a nameless message with custom input message.
        The log level of this function is critical.

      \param msg
        Custom message to log.

      \param logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      std::string LogCritical(std::string msg, bool logToFile = true);
    };

#include "ErrorLogger.tpp"
  }
}