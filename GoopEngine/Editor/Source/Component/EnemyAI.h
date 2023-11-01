#pragma once

#include <AI/Tree.h>


namespace GE
{
	namespace Component
	{
		using namespace GE::AI;

		struct EnemyAI
		{
			unsigned int m_entityID;
			GE::AI::TreeCache m_enemyTreeCache;


			EnemyAI(unsigned int entityID, TreeID treeID)
			{
				entityID = m_entityID;
				m_enemyTreeCache.m_treeID = treeID;
			}

			void RefreshCache()
			{
				m_enemyTreeCache.m_nodeCacheStack.clear();
			}
		};
	}
}
