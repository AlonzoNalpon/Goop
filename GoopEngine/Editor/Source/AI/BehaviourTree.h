#pragma once
#include <AI/Node.h>
#include <AI/ActionNode.h>
#include <AI/CompositeNode.h>



namespace GE
{
	namespace AI
	{

		struct BehaviourTree
		{
			Node* m_rootNode;
			void Tick();
			void Init(std::vector<Node>& children);
			void CleanUp();
		};


		NODE_STATES RootStart(std::vector<Node>&);
		void TestTree();
	}
}
