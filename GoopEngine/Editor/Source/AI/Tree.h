#pragma once
#include <vector>
#include <string>

namespace GE {
	namespace AI {

		using NodeID = unsigned int;
		using TreeID = unsigned int;

		enum NODE_TYPE
		{
			ROOT_NODE,
			COMPOSITE_NODE,
			LEAF_NODE,
			NODE_TYPE_COUNT
		};

		enum NODE_STATES {
			STATE_NEW,
			STATE_RUNNING,
			STATE_WAITING,
			STATE_SUCCEED,
			STATE_FAILED,
			STATE_COUNT
		};

		static std::vector<std::string> nodestateNames{ "NEW","RUNNING","WAITING","SUCCEED","FAILED","COUNT" };

		struct NodeCache
		{
			NodeID m_nodeID;
			int m_childIndex;
			NODE_STATES m_NodeResult;
		};

		struct TreeCache
		{
			TreeID m_treeID;
			std::deque<NodeCache> m_nodeCacheStack;
			NODE_STATES m_childResult;

		};







		//GameTree CreateTree(const std::vector<GE::AI::NodeTemplate>&);

	}
}