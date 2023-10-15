#pragma once
#include <ECS/System/System.h>
#include <Events/EventManager.h>

namespace GE
{
	namespace Systems
	{
		class DraggableObjectSystem : public GE::ECS::System, public GE::Events::IEventListener
		{
		public:
			void Update();

			/*!*********************************************************************
			\brief
			  Callback function to handle an event. Overriden from
				EventListener base class.
			\param event
				The event to handle
			************************************************************************/
			void HandleEvent(Events::Event const* event) override;

		private:
			bool isHeld = false;
		};
	}
}