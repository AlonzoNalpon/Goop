#pragma once
#include <ECS/System/System.h>
#include <Component/EnemyAI.h>
#include <AI/Tree.h>

namespace GE
{
	namespace Systems
	{
		void PrintNodeCache(const std::deque<GE::AI::NodeCache>& temp);

		class EnemySystem : public GE::ECS::System
		{

			static std::vector<GE::AI::Tree> m_treeList;
			static GE::AI::TreeID m_currentTreeID;
			static unsigned int m_playerID;
			static unsigned int m_currentEntityID;



		public:

			static void SetPlayerID(unsigned int playerID);

			static void UseTree(GE::AI::TreeID treeID, unsigned int entityID);

			static void InitTree(const std::vector<GE::AI::Tree>& treeList);

			void FixedUpdate();

			static int GetChildResult();

			static int GetCurrentChildIndex();

			static void SetResult(int result, GE::AI::NodeID currID);

			static void SetNewChildIndex(int index);

			static void RunChildNode(GE::AI::NodeID childNodeID);

			static void JumpToParent();

			static void ResetNode();

			static unsigned int GetPlayerID();


		};
	}
}
