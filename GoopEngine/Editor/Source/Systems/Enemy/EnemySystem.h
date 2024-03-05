/*!************************************************************************
\file EnemySystem.h
\author Han Qin Ding
\date  26-November-2023

\brief
This file contains function declaration for the Enemy System Class
The enemy system is in charge of running all the AI behaviours of the enemies.
Each behaviour tree can be used by multiple enemies. 
Instead of created 1 copy of the tree for each enemy, we will only have 1 copy of the tree at all times.
Each enemy will have a tree cache. This cache will be used to keep track of the enemy's progress within the behaivour tree.
Each enemy will only resolve 1 leaf node per frame, their cache will help the system determin which node the enemy will traverse
to in the current frame.

**************************************************************************/
#pragma once
#include <ECS/System/System.h>
#include <Component/EnemyAI.h>
#include <ScriptEngine/ScriptInstance.h>
#include <AI/TreeManager.h>

namespace GE
{
	namespace Systems
	{
		struct GameNode
		{
			GE::AI::NODE_TYPE m_nodeType;
			GE::MONO::ScriptInstance m_script;
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
			static bool m_playerExist;
			static unsigned int m_playerID;
			static unsigned int m_currentEntityID;





		public:
			/************************************************************************/ 
			/*!
			\brief
			Fixed update function for Enemy system. called every frame to update enemies' AI
			*/
			/************************************************************************/
			void FixedUpdate();

			/************************************************************************/ 
			/*!
			\brief
			Init function for enemy system. Gets the list of trees from tree manager and generate its own tree list for displaying.
			*/
			/************************************************************************/
			static void InitTree();

			/************************************************************************/ 
			/*!
			\brief
			Function to call when the enemy system wants to add newly generated tree into its own list of trees

			\param [treeTemp] const GE::AI::TreeTemplate&
			const reference to a tree tempate struct from tree manager
			*/
			/************************************************************************/
			static void AddGameTree(const GE::AI::TreeTemplate& treeTemp);

			/************************************************************************/ 
			/*!
			\brief
			Function to call when the enemy system wants to generate the tree from the tree manager's template
			the tree struct used by enemy system is quite different from the tree struct used by tree manager

			\param [treeTemp] const GE::AI::TreeTemplate&
			const reference to a tree tempate struct from tree manager

			\return
			Game tree struct that will be added to the enemy system's own list of trees
			*/
			/************************************************************************/
			static GameTree GenerateGameTree(const GE::AI::TreeTemplate& treeTemp);


			/************************************************************************/
			/*!
			\brief
			Function to call when the enemy system wants to remove trees that have been deleted through node editor

			\param [treeTemp] const GE::AI::TreeTemplate&
			const reference to a tree tempate struct from tree manager
			*/
			/************************************************************************/
			void DelGameTree();



			/************************************************************************/
			/*!
			\brief
			Function to call when the enemy system wants toupdate the tree list inside enemy system

			\param [treeTemp] const GE::AI::TreeTemplate&
			const reference to a tree tempate struct from tree manager
			*/
			/************************************************************************/
			void UpdateTreeList();


			void ReloadTrees();

			/************************************************************************/ 
			/*!
			\brief
			Function to call when an enemy wants to use a tree. The function will set the pointer to point to the 
			tree that the current enemy is using

			\param [treeID] GE::AI::TreeID 
			ID of the tree

			\param [entityID]  unsigned int
			ID of the enemy entity that is currently using the tree
			*/
			/************************************************************************/
			static void UseTree(GE::AI::TreeID treeID, unsigned int entityID);



			// /*!*********************************************************************
			//
			//    Internal Call Functions for C#
			//
			//************************************************************************/


			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions for getting the result of the child node

			\return
			int/enum representing the result of the child node
			*/
			/************************************************************************/
			static int GetChildResult();

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions for getting the index of the next child of the composite

			\return
			the index of the next child
			*/
			/************************************************************************/
			static int GetCurrentChildIndex();

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions called by a node to update the enemy's cache with its result after finishing executing. 

			\param [result] int
			result of the current node

			\param [currID]  GE::AI::NodeID
			ID of the current NOde

			*/
			/************************************************************************/
			static void SetResult(int result, GE::AI::NodeID currID);

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions called by a node to update the enemy's cache with the new child index 

			\param [result] int
			index of the next child in the current composite node
			*/
			/************************************************************************/
			static void SetNewChildIndex(int index);

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions called by a parent node to run its child node

			\param [childNodeID] GE::AI::NodeID
			ID of the child node
			*/
			/************************************************************************/
			static void RunChildNode(GE::AI::NodeID childNodeID);

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions called by a child node when it wants to jump to the parent node after finish executing
			*/
			/************************************************************************/
			static void JumpToParent();

			/************************************************************************/ 
			/*!
			\brief
			Function for refreshing the enemy's cache when he has finish traversng through the whole tree
			*/
			/************************************************************************/
			static void ResetNode();

			/************************************************************************/ /*!
			\brief
			Internal Call Functions called by child node when it wants to get the entity ID of the player

			\return
			ID of the player entity
			*/
			/************************************************************************/
			static unsigned int GetPlayerID();

			/************************************************************************/ 
			/*!
			\brief
			Internal Call Functions called by child node when it wants to check if the player entity exists in the game

			\return
			bool variable indicating if the player exist in the game
			*/
			/************************************************************************/
			static bool PlayerExist();


			static void StartAI(GE::ECS::Entity entityID);

			static void EndAI(GE::ECS::Entity entityID);

			/************************************************************************/ 
			/*!
			\brief
			 Function to call at the end of Each fixed update. This function will display the detail of the enemy's tree cache at
			 end of each frame
			*/
			/************************************************************************/
#ifdef _DEBUG
			static void PrintNodeCache(const std::deque<GE::AI::NodeCache>& temp);
#endif // _DEBUG

			//static void SetPlayerID();
		};


	}
}
