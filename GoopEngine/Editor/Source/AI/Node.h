#pragma once
#include <vector>
#include <string>
#include <ScriptEngine/Script.h>

namespace GE {
	namespace AI {
		using NodeID = unsigned int;

		struct NodeTemplate
		{
			NodeID m_parentNode;
			std::vector<NodeID> m_childrenNode;
			std::string m_scriptName;
		};

		struct Node
		{
			GE::MONO::Script m_script;
		};

	}
}
