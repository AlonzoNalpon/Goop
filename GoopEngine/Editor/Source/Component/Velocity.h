#pragma once

namespace GE
{
	namespace Component
	{
		using vec2 = GE::Math::dVec2;

		struct LinearForce
		{
			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			LinearForce(vec2 mag, double lifetime, double age = 0.0, bool active = true) : m_magnitude{mag}, m_lifetime{lifetime},
			m_age{age}, m_isActive {active} {}

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
			DragForce() : m_magnitude{}, m_isActive{} {}

			vec2 m_magnitude;
			bool m_isActive;
		};

		struct Velocity
		{
			vec2 m_vel;
			vec2 m_acc;
			double m_mass;
			vec2 m_gravity;
			DragForce m_dragForce;
			std::vector<LinearForce> m_forces;

			vec2 m_sumMagnitude;

			//forces functions
			void AddForce(vec2 mag, double lifetime, bool active = true)
			{
				LinearForce force(mag, lifetime, 0.0, active);
				m_forces.push_back(force);
			}

			void ActivateDrag(bool active)
			{
				m_dragForce.m_isActive = active;
			}

			//void RemoveForce();
		};
	}
}