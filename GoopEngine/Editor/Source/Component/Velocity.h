#pragma once

namespace GE
{
	namespace Component
	{
		using vec3 = GE::Math::dVec3;

		struct LinearForce
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			LinearForce() = default;

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			LinearForce(vec3 mag, double lifetime, bool active = false, double age = 0.f) : m_magnitude{ mag }, m_lifetime{ lifetime }, m_isActive{ active }, m_age{ age } {}

			vec3 m_magnitude;
			double m_lifetime;
			bool m_isActive;
			double m_age;
		};

		struct DragForce
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			DragForce() = default;

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			DragForce(double mag, bool active = true) : m_magnitude{ mag }, m_isActive{ active } {}

			double m_magnitude;
			bool m_isActive;
		};

		struct Velocity
		{
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Velocity() = default;

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Velocity(vec3 vel, vec3 acc, double mass, vec3 grav, DragForce drag = DragForce(0.9f), std::vector<LinearForce> forces = {}) :
				m_vel{ vel }, m_acc{ acc }, m_mass{ mass }, m_gravity{ grav }, m_dragForce{ drag }, m_forces{ forces } {}

			vec3 m_vel;
			vec3 m_acc;
			double m_mass;
			vec3 m_gravity;
			DragForce m_dragForce;
			std::vector<LinearForce> m_forces;

			static const vec3 m_threshold;

			vec3 m_sumMagnitude;

			//forces functions
			void AddForce(vec3 mag, double lifetime, bool active)
			{
				LinearForce force(mag, lifetime, active);
				m_forces.push_back(force);
			}
		};
	}
}