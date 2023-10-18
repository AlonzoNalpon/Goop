#include <pch.h>
#include <AI/BehaviourTree.h>


using namespace GE::AI;

void GE::AI::BehaviourTree::Tick()
{
	m_rootNode->m_behvaiourUpdate(m_rootNode->m_children);
}

void GE::AI::BehaviourTree::CleanUp()
{
  delete m_rootNode;
}

NODE_STATES GE::AI::RootStart(std::vector<Node>& children)
{
	//Root Node should only have 1 child. It will run that child's behaviour
	NODE_STATES result{ NODE_SUCCESS };
	result = children[0].m_behvaiourUpdate(children[0].m_children);
	return result;
}

void GE::AI::BehaviourTree::Init(std::vector<Node>& children)
{
	m_rootNode = new Node();
	m_rootNode->Init(children, RootStart, NODE_ROOT);
}

void GE::AI::TestTree()
{

  GE::AI::Node FailNode{};
  GE::AI::Node ScriptNode{};
  std::vector< GE::AI::Node>emptyVector;
  FailNode.Init(emptyVector, GE::AI::FailNodeUpdate, GE::AI::NODE_ACTION);
  ScriptNode.Init(emptyVector, GE::AI::ScriptUpdate, GE::AI::NODE_ACTION);

  std::vector< GE::AI::Node>Selector1{ ScriptNode,FailNode,FailNode };
  std::vector< GE::AI::Node>Selector2{ FailNode,ScriptNode,FailNode };
  std::vector< GE::AI::Node>Selector3{ FailNode,FailNode,ScriptNode };
  std::vector< GE::AI::Node>SelectorF{ FailNode,FailNode,FailNode };
  GE::AI::Node SelectorNode1{};
  GE::AI::Node SelectorNode2{};
  GE::AI::Node SelectorNode3{};
  GE::AI::Node SelectorNodeF{};
  SelectorNode1.Init(Selector1, GE::AI::SelectorUpdate, GE::AI::NODE_SELECTOR);
  SelectorNode2.Init(Selector2, GE::AI::SelectorUpdate, GE::AI::NODE_SELECTOR);
  SelectorNode3.Init(Selector3, GE::AI::SelectorUpdate, GE::AI::NODE_SELECTOR);
  SelectorNodeF.Init(SelectorF, GE::AI::SelectorUpdate, GE::AI::NODE_SELECTOR);

  std::vector< GE::AI::Node>Sequence1{ SelectorNode1,SelectorNode2,SelectorNode3,SelectorNodeF };
  GE::AI::Node SequenceNode{};
  SequenceNode.Init(Sequence1, GE::AI::SequenceUpdate, GE::AI::NODE_SEQUENCE);

  GE::AI::BehaviourTree aiTree{};


  std::vector< GE::AI::Node>treeChild{ SequenceNode };
  aiTree.Init(treeChild);
  aiTree.Tick();
  aiTree.CleanUp();

  

}
