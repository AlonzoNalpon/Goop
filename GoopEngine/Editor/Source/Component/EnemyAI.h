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
			GE::AI::TreeID m_treeID;


			EnemyAI(TreeID treeID)
			{
				m_treeID = treeID;
			}

			void RefreshCache()
			{
				m_enemyTreeCache.m_nodeCacheStack.clear();
			}
		};
	}
}
