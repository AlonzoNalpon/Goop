/*!************************************************************************
\file		EnemyAI.h
\author Han Qin Ding
\date   15-September-2023
\brief
	Component for Enemy AI. Any Enemy that wants to use a behaviour tree
	will need this component.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
**************************************************************************/
#pragma once

#include <AI/Tree.h>
#include <AI/TreeManager.h>
#include <iostream>


namespace GE
{
	namespace Component
	{
		using namespace GE::AI;

		const GE::AI::TreeID ghostTreeID = static_cast<GE::AI::TreeID>(-1); // ID for trees that were deleted while an enemy was using it

		struct EnemyAI
		{
			
			GE::AI::TreeCache m_enemyTreeCache;
			GE::AI::TreeID m_treeID;
			bool m_toTrigger{ true };
			

			EnemyAI() = default;
			EnemyAI(TreeID treeID) : m_treeID{ treeID }
			{
				std::vector<TreeTemplate>&  treeList = GE::AI::TreeManager::GetInstance().GetTreeList();
				auto iter = std::find_if(treeList.begin(), treeList.end(), [treeID](const GE::AI::TreeTemplate& t) -> bool
					{
						return t.m_treeTempID == treeID;
					});
				if (iter == treeList.end())
				{
					m_treeID = ghostTreeID;
					std::cout << "Tree DOesnt Exist\n";
				}
			
			}
			/*!*********************************************************************
			\brief
				Clear out the enemy's tree cache. to allow the enemy to traverse from
				the start of the tree again
			************************************************************************/
			void RefreshCache()
			{
				m_enemyTreeCache.m_nodeCacheStack.clear();
			}
		};
	}
}
