#include <pch.h>
#include <Commands/CommandManager.h>
#

using namespace GE;
using namespace CMD;

void CommandManager::AddCommand(rttr::variant newCmd)
{
	m_undoQueue.emplace_front(std::move(newCmd));
	m_redoQueue.clear();
	m_undoQueue.front().get_type().get_method("Execute").invoke(m_undoQueue.front());
}


void CommandManager::Undo()
{
	if (m_undoQueue.size() > 0)
	{
		m_undoQueue.front().get_type().get_method("Undo").invoke(m_undoQueue.front());
		m_redoQueue.emplace_front(std::move(m_undoQueue.front()));
		m_undoQueue.pop_front();
	}

}


void CommandManager::Redo()
{
	if (m_redoQueue.size() > 0)
	{
		m_redoQueue.front().get_type().get_method("Redo").invoke(m_redoQueue.front());
		m_undoQueue.emplace_front(std::move(m_redoQueue.front()));
		m_redoQueue.pop_front();
	}

}


