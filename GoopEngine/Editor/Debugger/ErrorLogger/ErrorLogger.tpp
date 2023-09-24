template <typename T>
ErrorLogger::LoggerPtr ErrorLogger::GetFileLogger()
{
  const char* typeName = typeid(T).name();
  // Logger type does not exist, create
  if (m_fileLoggers.find(typeName) == m_fileLoggers.end())
  {
    std::string formattedName{typeName};
    size_t pos1 = formattedName.rfind("::");
    size_t pos2 = formattedName.rfind(" ");

    if (pos1 == std::string::npos)
    {
      if (pos2 != std::string::npos)
      {
        formattedName = formattedName.substr(pos2 + 1, formattedName.length() - pos1);
      }
      //typeName is already the formatted name
    }
    else
    {
      formattedName = formattedName.substr(pos1 + 2, formattedName.length() - pos1 + 2);
    }

    m_fileLoggers[typeName] = std::make_shared<spdlog::logger>(formattedName, m_logDump);
  }

  return m_fileLoggers[typeName];
}

template <typename T>
ErrorLogger::LoggerPtr ErrorLogger::GetStreamLogger()
{
  const char* typeName = typeid(T).name();
  // Logger type does not exist, create
  if (m_streamLoggers.find(typeName) == m_streamLoggers.end())
  {
    std::string formattedName{typeName};
    size_t pos1 = formattedName.rfind("::");
    size_t pos2 = formattedName.rfind(" ");

    if (pos1 == std::string::npos)
    {
      if (pos2 != std::string::npos)
      {
        formattedName = formattedName.substr(pos2 + 1, formattedName.length() - pos1);
      }
      //typeName is already the formatted name
    }
    else
    {
      formattedName = formattedName.substr(pos1 + 2, formattedName.length() - pos1 + 2);
    }

    m_streamLoggers[typeName] = std::make_shared<spdlog::logger>(formattedName, m_logger);
  }

  return m_streamLoggers[typeName];
}

template <typename T>
std::string ErrorLogger::LogMessage(std::string msg, bool logToFile)
{
	// Don't log messages
	if (m_suppressLogWarning)
	{
		return msg;
	}

  if (logToFile)
  {
    LoggerPtr logger = GetFileLogger<T>();
    logger->info(msg);
  }
  LoggerPtr logger = GetStreamLogger<T>();
  logger->info(msg);

  return msg;
}

template <typename T>
std::string ErrorLogger::LogWarning(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    LoggerPtr logger = GetFileLogger<T>();
    logger->warn(msg);
  }
  LoggerPtr logger = GetStreamLogger<T>();
  logger->warn(msg);

  return msg;
}

template <typename T>
std::string ErrorLogger::LogError(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    LoggerPtr logger = GetFileLogger<T>();
    logger->error(msg);
  }
  LoggerPtr logger = GetStreamLogger<T>();
  logger->error(msg);

  return msg;
}

template <typename T>
std::string ErrorLogger::LogCritical(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    LoggerPtr logger = GetFileLogger<T>();
    logger->critical(msg);
  }
  LoggerPtr logger = GetStreamLogger<T>();
  logger->critical(msg);

  return msg;
}