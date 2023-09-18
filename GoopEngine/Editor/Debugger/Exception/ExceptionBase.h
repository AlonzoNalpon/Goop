#pragma once

#include <exception>
#include <string>

namespace GE
{
	namespace Debug
	{
		class IExceptionBase : public std::exception
		{
		public:
			/*!******************************************************************
			\brief 
			  Helps derived class to identify that they should log the exception
				with the template class name.

			\param[in] logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
			********************************************************************/
			virtual std::string LogSource(bool logToFile = true) = 0;

			/*!******************************************************************
			\brief 
			  Logs the exception.

			\param[in] logToFile
        Flag to indicate if this msg should be written to file.

			\return 
				Logged message.
			********************************************************************/
			virtual std::string Log(bool logToFile = true) = 0;
		};
	}
}
