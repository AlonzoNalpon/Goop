#pragma once
#include <AI/Node.h>

namespace GE {
	namespace AI {

		using TreeTemplate = std::vector<NodeTemplate>;
		using Tree = std::vector<Node>;
		using TreeID = unsigned int;


		enum NODE_STATES {
			STATE_NEW,
			STATE_RUNNING,
			STATE_WAITING,
			STATE_SUCCEED,
			STATE_FAILED,
			STATE_COUNT
		};

		static std::vector<std::string> nodestateNames{ "NEW","RUNNING","WAITING","SUCCEED","FAILED","COUNT"};

		struct NodeCache
		{
			NodeID m_nodeID;
			int m_childIndex;
			NODE_STATES m_NodeResult;
		};

		//Each user of the tree will have a cache to keep track of its progress
		struct TreeCache
		{
			TreeID m_treeID;
			std::deque<NodeCache> m_nodeCacheStack;
			NODE_STATES m_childResult;

		};

		Tree CreateTree(const TreeTemplate&);

	}
}