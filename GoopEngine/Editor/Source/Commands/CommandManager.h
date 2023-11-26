#pragma once
/*!************************************************************************
\file TreeManager.h
\author Han Qin Ding

\brief
This file contains function declaration for the TreeManager class
The Tree Manager class is in charge of serializing and deserializing the behaviour tree.
The node editor and the Enemy system will get Tree data from Tree Manager.
**************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include "../Singleton/Singleton.h"
#include <Commands/ImGuiCommands.h>
#include <deque>
#include <Math/GEM.h>
#include <rttr/type.h>

namespace GE {
	namespace CMD {

    const size_t MaxQueueSize{ 25 };

    RTTR_REGISTRATION{
     using namespace rttr;
     registration::class_<ICommand>("ICommand")
         .method("Execute", &ICommand::Execute)
         .method("Undo", &ICommand::Undo)
         .method("Redo", &ICommand::Redo);

     registration::class_<ChangeTransCmd>("ChangeTransCmd")
       .method("Execute", &ChangeTransCmd::Execute)
       .method("Undo", &ChangeTransCmd::Undo)
       .method("Redo", &ChangeTransCmd::Redo);

     registration::class_<AddObjectCmd>("AddObjectCmd")
       .method("Execute", &AddObjectCmd::Execute)
       .method("Undo", &AddObjectCmd::Undo)
       .method("Redo", &AddObjectCmd::Redo);

     registration::class_<RemoveObjectCmd>("RemoveObjectCmd")
       .method("Execute", &RemoveObjectCmd::Execute)
       .method("Undo", &RemoveObjectCmd::Undo)
       .method("Redo", &RemoveObjectCmd::Redo);

     //registration::class_<Derived2>("Derived2")
     //    .constructor<>()
     //    .property("value", &Derived2::value);
    }

		class CommandManager : public Singleton<CommandManager>
		{
      size_t queueSize{ MaxQueueSize };
			std::deque<rttr::variant> m_undoQueue;
			std::deque<rttr::variant> m_redoQueue;



		public:
			void AddCommand(rttr::variant newCmd);
      void Undo();
      void Redo();
		
		};
	}


}
#endif