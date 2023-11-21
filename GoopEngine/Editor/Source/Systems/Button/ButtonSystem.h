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
			void Start();

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
			// bool m_isClicked = false;
			GE::ECS::Entity m_buttonEntity;
			// std::string m_nextScene = "Start";
		};
	}
}