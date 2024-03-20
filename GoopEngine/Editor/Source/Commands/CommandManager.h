#pragma once
/*!*********************************************************************
\file   CommandManager.h
\author han.q\@digipen.edu
\date   26 November 2023
\brief
  Command Manager for our game engine. The command manager has queues to 
  keep track of the actions the made by the user. BUt storing certain
  actions inside the queues as commands, we allow user the user to undo and 
  redo actions.

  User can undo and redo Changing of transform, Adding of entity and deleting of entitys
  To Undo, press Ctrl-Z, to redo press Ctril-shift-Z.

  The undo and redo queue has a max size of 25, if user tries to add in more commands, the manager
  will pop put the oldest the commands from the queue, this means those old commands will not be able to
  be undo/redo


Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
    }

		class CommandManager : public Singleton<CommandManager>
		{
      size_t queueSize{ MaxQueueSize };
			std::deque<rttr::variant> m_undoQueue;
			std::deque<rttr::variant> m_redoQueue;



		public:
      /*!*********************************************************************
      \brief
        Function to add and execute a command. If the queue has already reached
        max capcity, the manager will remove the oldest command from the undo queue

       \param
        rttr::variant newCmd
        The new command that the user wants to execute.
      ************************************************************************/
			void AddCommand(rttr::variant newCmd);

      /*!*********************************************************************
      \brief
        Function to undo the latest action made by the user.
      ************************************************************************/
      void Undo();

      /*!*********************************************************************
      \brief
       Function to reddo the latest action undid by the user.
      ************************************************************************/
      void Redo();
		
		};
	}


}
#endif