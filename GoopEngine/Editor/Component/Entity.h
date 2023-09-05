#pragma once

#include <bitset>

#define MAX_COMPONENTS 32

class Entity
{
private:
	// A byte containing all flags of components
	std::bitset<MAX_COMPONENTS> m_componentFlag;
	unsigned int m_entityId;
};
