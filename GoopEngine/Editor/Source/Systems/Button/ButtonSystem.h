/*!*********************************************************************
\file   ButtonSystem.h
\author loh.j\@digipen.edu
\date   26-November-2023
\brief
	System to allow entities to do something after being interacted
	with a click.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/System/System.h>
// #include <Events/EventManager.h>
// #include <ECS/Entity/Entity.h>

namespace GE
{
	namespace Systems
	{
		class ButtonSystem : public GE::ECS::System, public GE::Events::IEventListener
		{
		public:
			/*!*********************************************************************
		\brief
			Starts the system and subscribes to events
		************************************************************************/
			void Start();

			/*!*********************************************************************
			\brief
				Update function of the system. Checks if a button was selected on
				and runs the corresponding event.
			************************************************************************/
			void Update();

			/*!*********************************************************************
			\brief
				Callback function to handle an event. Overriden from
				EventListener base class.
			\param event
				The event to handle
			************************************************************************/
			void HandleEvent(Events::Event* event) override;

		private:
			bool m_shouldHandle;
			GE::ECS::Entity m_buttonEntity;	
		};
	}
}