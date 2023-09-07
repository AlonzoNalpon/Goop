#pragma once

namespace GE
{
	namespace ECS
	{
		// Base for polymoprhic components
		class Component
		{
		private:
			bool m_isActive;
		public:
			Component() : m_isActive{ true } {}
			~Component() = default;

			inline const bool GetActive() const { return m_isActive; };
			inline void SetActive(const bool active) { m_isActive = active; }
		};
	}
}
