#include <pch.h>
#include <AI/CompositeNode.h>


using namespace GE::AI;

NODE_STATES  GE::AI::SelectorUpdate(std::vector<Node>& children)
{
	std::cout << "SelectorNode\n";
	std::cout << "----------------------------------------------\n";
	NODE_STATES result{NODE_FAILURE};
	for (Node& node : children)
	{
		if (result == NODE_FAILURE)
		{
			result = node.m_behvaiourUpdate(node.m_children);
		}
	}
	std::string finalresult = (result == NODE_FAILURE) ? "FAIL" : "SUCCESS";
	std::cout << "Selector final result: " << finalresult << "\n";
	std::cout << "----------------------------------------------\n\n";
	return result;
}

NODE_STATES  GE::AI::SequenceUpdate(std::vector<Node>& children)
{
	std::cout << "SequenceNode\n";
	std::cout << "[\n";
	NODE_STATES result{ NODE_SUCCESS };
	for (Node& node : children)
	{
		if (result == NODE_SUCCESS)
		{
			result = node.m_behvaiourUpdate(node.m_children);
		}
	}
	std::string finalresult = (result == NODE_FAILURE) ? "FAIL" : "SUCCESS";
	std::cout << "Sequence final result: " << finalresult << "\n";
	std::cout << "]\n";
	return result;
}
