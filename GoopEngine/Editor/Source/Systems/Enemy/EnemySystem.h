#pragma once
#include <ECS/System/System.h>
#include <Component/EnemyAI.h>
#include <ScriptEngine/Script.h>
#include <ScriptEngine/ScriptManager.h>
#include <AI/TreeManager.h>

namespace GE
{
	namespace Systems
	{
		struct GameNode
		{
			GE::AI::NODE_TYPE m_nodeType;
			GE::MONO::Script m_script;
		};

		struct GameTree
		{
			std::vector<GameNode> m_nodeList;
			GE::AI::TreeID m_treeID;
		};


		class EnemySystem : public GE::ECS::System
		{
			const std::string rootNodeName = "RootNode";
			static std::vector<GameTree> m_treeList;
			static GameTree* m_currentTree;
			static GE::AI::TreeID m_currentTreeID;
			static unsigned int m_playerID;
			static unsigned int m_currentEntityID;





		public:

			void FixedUpdate();

			static void InitTree();

			static void AddGameTree(const GE::AI::TreeTemplate& treeTemp);

			static GameTree GenerateGameTree(const GE::AI::TreeTemplate& treeTemp);

			static void SetPlayerID(unsigned int playerID);

			static void UseTree(GE::AI::TreeID treeID, unsigned int entityID);


			// /*!*********************************************************************
			//
			//    Internal Call Functions for C#
			//
			//************************************************************************/

			static int GetChildResult();

			static int GetCurrentChildIndex();

			static void SetResult(int result, GE::AI::NodeID currID);

			static void SetNewChildIndex(int index);

			static void RunChildNode(GE::AI::NodeID childNodeID);

			static void JumpToParent();

			static void ResetNode();

			static unsigned int GetPlayerID();

			static void SetPlayerID();


		};

		void PrintNodeCache(const std::deque<GE::AI::NodeCache>& temp);
	}
}
