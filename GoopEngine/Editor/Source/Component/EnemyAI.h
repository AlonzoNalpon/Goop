/*!************************************************************************
\file EnemyAI.h
\author Han Qin Ding

\brief
Component for Enemy AI. Any Enemy that wants to use a behaviour tree, will have
this component. 
**************************************************************************/
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

			EnemyAI() = default;
			EnemyAI(TreeID treeID) : m_treeID{ treeID }
			{
				std::cout << "\n\nNFOQNFNWOUIFWNOIFWOIUNFFWFWE\n\n";
			}
			/************************************************************************/ /*!
			\ brief
				Clear out the enemy's tree cache. to allow the enemy to traverse from the start of the tree again
			*/
			/************************************************************************/
			void RefreshCache()
			{
				m_enemyTreeCache.m_nodeCacheStack.clear();
			}
		};
	}
}
