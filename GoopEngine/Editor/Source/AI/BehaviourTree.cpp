#include <AI/BehaviourTree.h>



using namespace GE::AI;

void BehaviourTree::Tick()
{
	m_rootNode->m_behvaiourUpdate(m_rootNode->m_children);
}



NODE_STATES GE::AI::RootStart(std::vector<Node>& children)
{
	//Root Node should only have 1 child. It will run that child's behaviour
	NODE_STATES result{ NODE_SUCCESS };
	result = children[0].m_behvaiourUpdate(children[0].m_children);
	return result;
}

void BehaviourTree::Init(std::vector<Node>& children)
{
	m_rootNode = new Node();
	m_rootNode->Init(children, RootStart, NODE_ROOT);
}