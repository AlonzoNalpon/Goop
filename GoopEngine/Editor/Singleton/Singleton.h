/*!******************************************************************
\file   Singleton.h 
\author w.chinkitbryam\@digipen.edu
\date   6 September 2023
\brief  
  
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
		  
		********************************************************************/
		Singleton() = default;
		virtual ~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	public:
		static T& GetInstance();
	};

#include "Singleton.tpp"
}
