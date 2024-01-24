/*!*********************************************************************
\file   Velocity.h
\author c.phua\@digipen.edu
\date   8 November 2023
\brief
	Component for Linear Force and Velocity.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
			LinearForce(const vec3 & mag, double lifetime, bool active = false, double age = 0.f) : m_magnitude{ mag }, m_lifetime{ lifetime }, m_isActive{ active }, m_age{ age } {}


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
			Velocity(const vec3& vel, const vec3& acc, double mass, const vec3& grav, DragForce drag = DragForce(0.9f), std::vector<LinearForce> forces = {}) :
				m_vel{ vel }, m_acc{ acc }, m_mass{ mass }, m_gravity{ grav }, m_dragForce{ std::move(drag) }, m_forces{ std::move(forces) } {}

			vec3 m_vel;
			vec3 m_acc;
			double m_mass{1.0};
			vec3 m_gravity;
			DragForce m_dragForce;
			std::vector<LinearForce> m_forces;

			vec3 m_sumMagnitude;

			/*!*********************************************************************
			\brief
				Creates a linear force and adds it into a vector of forces
			\param mag
				magnitude of force -> in a double vec3
			\param lifetime
				lifetime of force i.e. how long the force will last -> in seconds
			\param active
				bool to activate force
			************************************************************************/
			void AddForce(const vec3& mag, double lifetime, bool active)
			{
				LinearForce force(mag, lifetime, active);
				m_forces.push_back(std::move(force));
			}

			/*!*********************************************************************
			\brief
				Gets the magnitude of a vector
			\param mag
				vector to compute magnitude of
			\return
				the magnitude of the vector -> in double
			************************************************************************/
			double GetMagnitude(const vec3& mag)
			{
				return ((mag.x * mag.x) + (mag.y * mag.y));
			}
		};
	}
}