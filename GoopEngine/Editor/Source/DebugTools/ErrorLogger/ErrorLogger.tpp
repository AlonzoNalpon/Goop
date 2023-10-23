template <typename T>
std::string ErrorLogger::LogMessage(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_wroteToFile = true;
    m_logger->info(typeid(T).name() + (": " + msg));
  }
  else
  {
    m_fileLogger->info(typeid(T).name() + (": " + msg));
  }

  return msg;
}

template <typename T>
std::string ErrorLogger::LogWarning(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_wroteToFile = true;
    m_logger->warn(typeid(T).name() + (": " + msg));
  }
  else
  {
    m_fileLogger->warn(typeid(T).name() + (": " + msg));
  }

  return msg;
}

template <typename T>
std::string ErrorLogger::LogError(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_wroteToFile = true;
    m_logger->error(typeid(T).name() + (": " + msg));
  }
  else
  {
    m_fileLogger->error(typeid(T).name() + (": " + msg));
  }

  return msg;
}

template <typename T>
std::string ErrorLogger::LogCritical(std::string msg, bool logToFile)
{
  if (logToFile)
  {
    m_wroteToFile = true;
    m_logger->critical(typeid(T).name() + (": " + msg));
  }
  else
  {
    m_fileLogger->critical(typeid(T).name() + (": " + msg));
  }

  return msg;
}