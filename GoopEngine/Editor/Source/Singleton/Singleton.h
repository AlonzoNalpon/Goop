/*!******************************************************************
\file   Singleton.h 
\author w.chinkitbryan\@digipen.edu
\date   6-September-2023
\brief  
  Singleton template class. Manages a "global" instance of any
	class of type Singleton.
********************************************************************/
#pragma once

namespace GE
{
	template <typename T>
	class Singleton
	{
	protected:
		/*!******************************************************************
		\brief 
		  Default constructor is called during GetInstance
		********************************************************************/
		Singleton() = default;

		/*!*********************************************************************
		\brief
		  Calls derived destructor when obj leaves program scope.
		************************************************************************/
		virtual ~Singleton() = default;

		/*!*********************************************************************
		\brief
		  No copy constructor for singleton objects, every singleton object
			should be only gotten from GetInstance.
		************************************************************************/
		Singleton(const Singleton&) = delete;

		/*!*********************************************************************
		\brief
		  No copy assignment for singleton object, every singleton object
			should either be a reference or a pointer to the singleton object.
		************************************************************************/
		Singleton& operator=(const Singleton&) = delete;

	public:
		/*!*********************************************************************
		\brief
		  Returns a static instance of the singleton class.
		  
		\return
		  Static instance of class.
		************************************************************************/
		static T& GetInstance();
	};

#include "Singleton.tpp"
}
