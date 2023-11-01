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

			EnemyAI(unsigned int entityID, GE::AI::TreeCache const& enemyTreeCache)
			{
				entityID = m_entityID;
				m_enemyTreeCache = enemyTreeCache;
			}

			void RefreshCache()
			{
				while (m_enemyTreeCache.m_nodeCacheStack.size() > 1)
				{
					m_enemyTreeCache.m_nodeCacheStack.pop_front();
				}
				m_enemyTreeCache.m_nodeCacheStack.front().m_childIndex = 0;
				m_enemyTreeCache.m_nodeCacheStack.front().m_NodeResult = STATE_NEW;
			}
		};
	}
}
