#pragma once

namespace GE
{
	template <typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	public:
		static T& GetInstance();
	};

#include "Singleton.tpp"
}
