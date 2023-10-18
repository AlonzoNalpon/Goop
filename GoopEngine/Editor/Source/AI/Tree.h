#pragma once
#include <vector>
#include <string>

namespace GE {
	namespace AI {
		using NodeID = unsigned int;

		struct NodeTemplate
		{
			NodeID m_parent;
			std::vector<NodeID> m_children;
			std::string m_scriptName;
		};


	}
}
