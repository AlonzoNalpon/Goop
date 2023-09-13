/*!*********************************************************************
\file   Component.h
\author w.chinkitbryan\@digipen.edu
\date   6-September-2023
\brief  
  Base class components. This is optional but recommended as components
	should be allowed to be activated or deactivated. This does nothing
	if the system does not check if its active or not.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

namespace GE
{
	namespace ECS
	{
		// Base for components to allow basic toggling feature
		// Inheriting from this class is entirely optional
		class Component
		{
		private:
			bool m_isActive;
		public:
			/*!*********************************************************************
			\brief
			  Default constructor. By default sets entities as active.
			************************************************************************/
			Component() : m_isActive{ true } {}
			/*!*********************************************************************
			\brief
			  Defaulted constructor.
			************************************************************************/
			~Component() = default;

			/*!******************************************************************
			\brief
			  Returns the state of the component.

			\return
				Current active state of component.
			********************************************************************/
			inline const bool GetActive() const { return m_isActive; };
			/*!*********************************************************************
			\brief
			  Sets the state of the component.

			\param active
				The state to set the component to.
			************************************************************************/
			inline void SetActive(const bool active) { m_isActive = active; }
		};
	}
}
