#pragma once
#include <pch.h>

namespace GE
{
	namespace Component
	{
		using vec2 = GE::Math::dVec2;

		enum class Forces
		{
			LINEARFORCE,
			DRAGFORCE,
			GRAVITY,
		};

		struct LinearForce
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			LinearForce() : m_direction{}, m_magnitude{}, m_lifetime{}, m_age{}, m_isActive{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			LinearForce(vec2 dir, vec2 mag, double lifetime, double age = 0, bool active = false) : m_direction{dir}, m_magnitude{mag}, m_lifetime{lifetime},
			m_age{age}, m_isActive{active}
			{
			}

			vec2 m_direction;
			vec2 m_magnitude;
			double m_lifetime;
			double m_age;
			bool m_isActive;
		};

		struct DragForce
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			DragForce() : m_direction{}, m_magnitude{}, m_isActive{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			DragForce(vec2 dir, vec2 mag, bool active = false) : m_direction{ dir }, m_magnitude{ mag }, m_isActive{ active }
			{
			}

			vec2 m_direction;
			vec2 m_magnitude;
			bool m_isActive;
		};

		struct Gravity
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Gravity() : m_gravity{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Gravity(vec2 grav) : m_gravity{ grav }
			{
			}

			vec2 m_gravity;
		};

		struct Velocity
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Velocity() : m_acc{}, m_vel{}, m_mass{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Velocity(vec2 acc, vec2 vel, double mass) : m_acc{ acc }, m_vel{ vel }, m_mass{ mass }
			{
			}

			vec2 m_acc; //starting off as stationary
			vec2 m_vel;
			double m_mass;
		};

		struct ForcesManager
		{
		public:
			void AddForce(Forces force) 
			{

			}

			void RemoveForce(Forces force)
			{

			}

			void ActivateForce(Forces force, bool active)
			{

			}

			double ValidateAge(Forces force)
			{

			}

			double SetLifeTime(Forces force)
			{

			}

		private:
			vec2 m_finalDirection;
			vec2 m_sumMagnitude;
		};
	}
}