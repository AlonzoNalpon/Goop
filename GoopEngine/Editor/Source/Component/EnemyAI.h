#pragma once

#include <AI/Tree.h>


namespace GE
{
	namespace Component
	{
		using namespace GE::AI;

		struct EnemyAI
		{
			GE::AI::TreeCache m_enemyTreeCache;


			EnemyAI(TreeID treeID)
			{
				m_enemyTreeCache.m_treeID = treeID;
			}

			void RefreshCache()
			{
				m_enemyTreeCache.m_nodeCacheStack.clear();
			}
		};
	}
}
