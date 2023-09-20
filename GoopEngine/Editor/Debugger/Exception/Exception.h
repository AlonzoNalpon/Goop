/*!******************************************************************
\file   Exception.h
\author w.chinkitbryam\@digipen.edu
\date   17 September 2023
\brief
  Custom exception class with spdlog integration.
********************************************************************/

#pragma once
#include "ExceptionBase.h"
#include <string>
#include "../ErrorLogger/ErrorLogger.h"

#define ERRLG_FILE __FILE__
#define ERRLG_FUNC __FUNCTION__
#define ERRLG_LINE __LINE__

#define ErrMsg(msg) msg, ERRLG_FUNC, ERRLG_LINE

namespace GE
{
  namespace Debug
  {
    enum EXCEPTION_LEVEL
    {
      LEVEL_INFO = 0,     // Slap on the wrist level
      LEVEL_WARN,     // This may be a potential issue
      LEVEL_ERROR,    // This can cause a crash
      LEVEL_CRITICAL, // This will cause a crash
    };

    template <typename T>
    class Exception : public IExceptionBase
    {
    private:
      std::string m_errMsg{ "Unknown Error" };
      std::string m_loc{"Unknown Location"};

      // Error line
      int m_line;
      EXCEPTION_LEVEL m_lvl;

      // Singleton pointer
      ErrorLogger& m_el;

      /*!******************************************************************
      \brief 
        Constructs the error message in a custom format based on the
        parameters which the exception object was created.

      \return
        Formatted string.
      ********************************************************************/
      std::string ConstructMessage();
    public:
      // Must log error with string
      Exception() = delete;
      /*!******************************************************************
      \brief 
        Creates an exception with all required values for a custom message.
        Please throw the system in which the object throwing belongs to where
        sufficient rather than individual class whenever appropriate.
        This is to avoid too many unique thrown identifies which uses
        more memory in the logger's map.

      \param[in] lvl
        Logging level

      \param[in] err
        What the error is

      \param[in] loc
        File or function. 
        Inputs are either;
        ERRLG_FILE
        ERRLG_FUNC

      \param[in] line
        Line of error
        Inputs can only be;
        ERRLG_LINE
      ********************************************************************/
      Exception(EXCEPTION_LEVEL lvl, std::string err, std::string loc, int line);
      // Copy ctor & copy assignment are default
       
      // what() is not defined!!!
      // use Log functions below
      
      /*!******************************************************************
      \brief 
        Replacement for exception what() which will interface with 
        errorlogger to call spdlog. This function will let you also log 
        the object which the exception was created for. 
        
      \param[in] logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
      ********************************************************************/
      std::string LogSource(bool logToFile = true);

      /*!******************************************************************
      \brief
        Replacement for exception what() which will interface with
        errorlogger to call spdlog.

      \param[in] logToFile
        Flag to indicate if this msg should be written to file.

      \return
        Logged message.
      ********************************************************************/
      std::string Log(bool logToFile = true);
    };

#include "Exception.tpp"
  }
}
