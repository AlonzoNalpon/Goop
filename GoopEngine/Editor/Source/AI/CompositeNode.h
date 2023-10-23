#pragma once
#include "../AI/Node.h"



namespace GE
{
	namespace AI
	{

		NODE_STATES SelectorUpdate(std::vector<Node>&);

		NODE_STATES SequenceUpdate(std::vector<Node>&);

	}
}
