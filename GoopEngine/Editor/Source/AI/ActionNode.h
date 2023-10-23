#pragma once
#include "../AI/Node.h"
#include <vector>


namespace GE
{
	namespace AI
	{

		NODE_STATES FailNodeUpdate(std::vector<Node>&);

		NODE_STATES ScriptUpdate(std::vector<Node>&);

	}
}
