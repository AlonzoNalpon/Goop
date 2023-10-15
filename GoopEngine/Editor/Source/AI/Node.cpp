#include <AI/Node.h>


using namespace GE::AI;


void Node::Init(std::vector<Node>& children, NodeUpdateFP funcPointer, NODE_TYPE nodeType, NODE_STATES nodeStatus) 
{
	m_children = children ;
	m_behvaiourUpdate = funcPointer ;
	m_nodeType = nodeType;
	m_nodeStatus = nodeStatus;
}

void Node::Terminate(NODE_STATES)
{

}

void Node::AddChild(const Node& childNode)
{
	m_children.push_back(childNode);
}
