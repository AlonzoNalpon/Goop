/*!*********************************************************************
\file   CommandManager.cpp
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


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include <Commands/CommandManager.h>



using namespace GE;
using namespace CMD;

void CommandManager::AddCommand(rttr::variant newCmd)
{
	m_undoQueue.emplace_front(std::move(newCmd));
	m_redoQueue.clear();
	m_undoQueue.front().get_type().get_method("Execute").invoke(m_undoQueue.front());
	if (m_undoQueue.size() > queueSize)
	{
		m_undoQueue.pop_back();
	}
}


void CommandManager::Undo()
{
	if (m_undoQueue.size() > 0)
	{
		m_undoQueue.front().get_type().get_method("Undo").invoke(m_undoQueue.front());
		m_redoQueue.emplace_front(std::move(m_undoQueue.front()));
		m_undoQueue.pop_front();

		if (m_redoQueue.size() > queueSize)
		{
			m_redoQueue.pop_back();
		}
	}
}


void CommandManager::Redo()
{
	if (m_redoQueue.size() > 0)
	{
		m_redoQueue.front().get_type().get_method("Redo").invoke(m_redoQueue.front());
		m_undoQueue.emplace_front(std::move(m_redoQueue.front()));
		m_redoQueue.pop_front();

		if (m_undoQueue.size() > queueSize)
		{
			m_undoQueue.pop_back();
		}
	}

}


#endif