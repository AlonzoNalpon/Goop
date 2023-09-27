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
std::string Exception<T>::LogSource(bool logToFile)
{
  switch (m_lvl)
  {
  case LEVEL_WARN:
    return m_el.LogWarning<T>(m_errMsg, logToFile);
    break;
  case LEVEL_ERROR:
    return m_el.LogError<T>(m_errMsg, logToFile);
    break;
  case LEVEL_CRITICAL:
    return m_el.LogCritical<T>(m_errMsg, logToFile);
    break;
  default: // Default will assume throw is just info
  case LEVEL_INFO:
    return m_el.LogMessage<T>(m_errMsg, logToFile);
    break;
  }
}

template <typename T>
std::string Exception<T>::Log(bool logToFile)
{
  switch (m_lvl)
  {
  case LEVEL_WARN:
    return m_el.LogWarning(m_errMsg, logToFile);
    break;
  case LEVEL_ERROR:
    return m_el.LogError(m_errMsg, logToFile);
    break;
  case LEVEL_CRITICAL:
    return m_el.LogCritical(m_errMsg, logToFile);
    break;
  default: // Default will assume throw is just info
  case LEVEL_INFO:
    return m_el.LogMessage(m_errMsg, logToFile);
    break;
  }
}