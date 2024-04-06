/*!************************************************************************
\file EnemySystem.cpp
\author Han Qin Ding
\date  26-November-2023

\brief
	This file contains functions for the Enemy System Class
	The enemy system is in charge of running all the AI behaviours of the
	enemies.
	Each behaviour tree can be used by multiple enemies.
	Instead of created 1 copy of the tree for each enemy, we will only have 
	1 copy of the tree at all times.
	Each enemy will have a tree cache. This cache will be used to keep track
	of the enemy's progress within the behaivour tree.
	Each enemy will only resolve 1 leaf node per frame, their cache will help
	the system determin which node the enemy will traverse to in the current
	frame.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
**************************************************************************/
#include <pch.h>
#include <Systems/Enemy/EnemySystem.h>
#include <ScriptEngine/ScriptManager.h>
#include <AssetManager/AssetManager.h>

using namespace GE;
using namespace AI;
using namespace ECS;
using namespace Systems;


unsigned int EnemySystem::m_currentEntityID;
unsigned int EnemySystem::m_currentTreeID;
unsigned int EnemySystem::m_playerID;
std::vector<GameTree> EnemySystem::m_treeList;
GameTree* EnemySystem::m_currentTree;
bool EnemySystem::m_playerExist;


void EnemySystem::InitTree()
{
	for (GE::Systems::GameTree& gt : m_treeList)
	{
		gt.ClearAllNodes();
	}
	EnemySystem::m_treeList.clear();
	//Get all the trees
	const std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
	for (size_t i{ 0 }; i < tempTreeList.size(); ++i)
	{
		AddGameTree(tempTreeList[i]);
	}
	// Set the first tree as the current tree
	m_currentTree = (m_treeList.size() != 0) ? &(m_treeList[0]) : nullptr;
}

void EnemySystem::ReloadTrees()
{
	std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
	for (TreeTemplate& tt : tempTreeList)
	{
		std::vector<GameTree>::iterator iter = std::find_if(m_treeList.begin(), m_treeList.end(), [tt](GameTree& tree) -> bool
			{
				return tree.m_treeID == tt.m_treeTempID;
			});

		if (iter != m_treeList.end()) //If we already have that tree in our list, we will just swap
		{
			GameTree newGamTree = GenerateGameTree(tt);
			std::swap(iter->m_nodeList, newGamTree.m_nodeList);

		}
		else // We got a completely new tree, we will just add it into the system
		{
			AddGameTree(tt);
		}

		for (Entity entity : GetUpdatableEntities()) {
			GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
			GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
			if (enemyAIComp->m_treeID == tt.m_treeTempID)
			{
				enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.clear(); // Clear the cache since the tree is updated
			}
		}
	}
}

void EnemySystem::FixedUpdate()
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();

#ifndef IMGUI_DISABLE
	//The tree list has been updated with node editor, we should update the tree list inside the enemy system too
	if (GE::AI::TreeManager::GetInstance().isTreeUpdated())
	{
		DelGameTree();
		UpdateTreeList();
		GE::AI::TreeManager::GetInstance().SetTreeBool(false);

	}
#endif

	// Only update if there are actual trees
	if (m_treeList.size() != 0)
	{
		for (Entity entity : GetUpdatableEntities()) {
			
			GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
			GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
			GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
			if (enemyAIComp->m_treeID != GE::Component::ghostTreeID && enemyAIComp->m_toTrigger)
			{
				UseTree(enemyAIComp->m_treeID, entity);

				//If the nodeCacheStack is empty, it means that the enemy is going to traverse from the start of the tree again
				if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.size() == 0)
				{
					for (size_t j{ 0 }; j < m_currentTree->m_nodeList.size(); ++j)
					{
						if (m_currentTree->m_nodeList[j].m_nodeType == ROOT_NODE)
						{
							enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.push_back(NodeCache(static_cast<NodeID>(j), 0, STATE_NEW));
							break;
						}
					}
				}
				if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.size() != 0)
				{
					double dt = fpsControl->GetFixedDeltaTime();
					std::vector<void*> arg{ &m_currentEntityID, &dt };
					MonoMethod* onUpdateMethod = mono_class_get_method_from_name(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_scriptClass, "OnUpdate", static_cast<int>(arg.size()));
					mono_runtime_invoke(onUpdateMethod, mono_gchandle_get_target(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_gcHandle), arg.data(), nullptr);
					//PLEASE UNCOMMENT THIS IF YOU WANT TO SEE THE TREE CACHE OF THE ENEMY AT THE END OF EVRY FRAME
					//PrintNodeCache(enemyAIComp->m_enemyTreeCache.m_nodeCacheStack);
				}
				else
				{
					GE::Debug::ErrorLogger::GetInstance().LogWarning("Your tree has no root node, please a root node to the start of the tree", false);
				}
			}
		}
	}
	frc.EndSystemTimer("Enemy AI");
}


void EnemySystem::AddGameTree(const GE::AI::TreeTemplate& treeTemp)
{
	GameTree newGamTree = GenerateGameTree(treeTemp);

	m_treeList.push_back(newGamTree);
}

GameTree EnemySystem::GenerateGameTree(const GE::AI::TreeTemplate& treeTemp)
{
	
	//GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
	const std::vector<NodeTemplate>& tree = treeTemp.m_tree;

	GameTree newGamTree{ {},treeTemp.m_treeTempID };
	//std::cout << "\nNEW TREE---------------------------------\n";
	//Loop through each node in the tree and create a new GameNode
	for (size_t i{ 0 }; i < tree.size(); ++i)
	{
		NodeID ownID = static_cast<unsigned int>(i);
		NodeID parentID = tree[i].m_parentNode;
		unsigned int listSize = static_cast<unsigned int>(tree[i].m_childrenNode.size());
		//std::cout << tree[i].m_scriptName.c_str()  << ":" << ownID << "\n";

		MonoArray* result = mono_array_new(mono_domain_get(), mono_get_uint32_class(), listSize);
		for (unsigned int j = 0; j < listSize; j++) {
			mono_array_set(result, unsigned int, j, tree[i].m_childrenNode[j]);
		}
		std::vector<void*> arg{ &ownID, &parentID, result, &listSize };
		GE::MONO::ScriptInstance scriptInst = GE::MONO::ScriptInstance(tree[i].m_scriptName.c_str(), arg);
		newGamTree.m_nodeList.push_back(GameNode(tree[i].m_nodeType, scriptInst));
	}
	//std::cout << "---------------------------------------------\n";
	return newGamTree;
}

void EnemySystem::UseTree(TreeID treeID, unsigned int entityID)
{
	m_currentEntityID = entityID;
	m_currentTreeID = treeID;
	for (GE::Systems::GameTree& gt : EnemySystem::m_treeList)
	{
		m_currentTree = (gt.m_treeID == treeID) ? &gt : m_currentTree;
	}
}

int EnemySystem::GetChildResult()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	return static_cast<int>(enemyAIComp->m_enemyTreeCache.m_childResult);
}

int EnemySystem::GetCurrentChildIndex()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	return enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_childIndex;
}


void EnemySystem::SetResult(int result, GE::AI::NodeID currID)
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	enemyAIComp->m_enemyTreeCache.m_childResult = static_cast<GE::AI::NODE_STATES>(result);

	for (GE::AI::NodeCache& n : enemyAIComp->m_enemyTreeCache.m_nodeCacheStack)
	{
		n.m_NodeResult = (n.m_nodeID == currID) ? static_cast<GE::AI::NODE_STATES>(result) : n.m_NodeResult;
	}
}


void EnemySystem::SetNewChildIndex(int index)
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_childIndex = index;
}

//
void EnemySystem::RunChildNode(GE::AI::NodeID childNodeID)
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.push_front(NodeCache(childNodeID, 0, NODE_STATES::STATE_NEW));

	double dt = fpsControl->GetDeltaTime();
	std::vector<void*> arg{ &m_currentEntityID, &dt };
	MonoMethod* onUpdateMethod = mono_class_get_method_from_name(m_currentTree->m_nodeList[childNodeID].m_script.m_scriptClass, "OnUpdate", static_cast<int>(arg.size()));
	mono_runtime_invoke(onUpdateMethod, mono_gchandle_get_target(m_currentTree->m_nodeList[childNodeID].m_script.m_gcHandle), arg.data(), nullptr);


}




void EnemySystem::JumpToParent()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);

	if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.size() <= 1)
	{
		GE::Debug::ErrorLogger::GetInstance().LogWarning("You have assigned the wrong script to the node", false);
	}
	else
	{
		enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.pop_front();

		if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_NodeResult == STATE_WAITING)
		{
			std::vector<void*> arg{ &m_currentEntityID };
			MonoMethod* onUpdateMethod = mono_class_get_method_from_name(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_scriptClass, "ReturnFromChild", static_cast<int>(arg.size()));
			mono_runtime_invoke(onUpdateMethod, mono_gchandle_get_target(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_gcHandle), arg.data(), nullptr);
		}
	}
}

void EnemySystem::ResetNode()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);
	enemyAIComp->RefreshCache();
}


unsigned int  EnemySystem::GetPlayerID()
{
	return m_playerID;
}

#ifdef _DEBUG
void EnemySystem::PrintNodeCache(const std::deque<GE::AI::NodeCache>& temp)
{
	std::cout << "Tree Cache Detail:\n";
	std::cout << "-----------------------\n";
	for (const GE::AI::NodeCache& n : temp)
	{
		std::cout << GE::AI::nodeTitles[static_cast<int>(m_currentTree->m_nodeList[n.m_nodeID].m_nodeType)] << ": " << GE::AI::nodestateNames[n.m_NodeResult] << "\n";
	}
	std::cout << "-----------------------\n\n";
}
#endif // _DEBUG

bool EnemySystem::PlayerExist()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	std::set<GE::ECS::Entity> entityList = ecs->GetEntities();
	bool hasPlayer = false;
	for (GE::ECS::Entity e : entityList)
	{
		if (ecs->GetEntityName(e) == "Player")
		{
			hasPlayer = true;
			m_playerID = e;
			break;
		}
	}
	return hasPlayer;
}

void EnemySystem::StartAI(GE::ECS::Entity entityID)
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entityID);
	enemyAIComp->m_toTrigger = true;
}

void EnemySystem::EndAI(GE::ECS::Entity entityID)
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entityID);
	enemyAIComp->m_toTrigger = false;
	enemyAIComp->RefreshCache();
}

void GameTree::ClearAllNodes()
{
	for (GameNode& g : m_nodeList)
	{
		g.m_script.FreeScript();
	}

}

void EnemySystem::DelGameTree()
{
	std::vector<TreeID>& delTrees = GE::AI::TreeManager::GetInstance().GetDelTreeList();
	for (TreeID t : delTrees)	//Loop through the list of trees, remove any unwanted trees
	{
		auto iter = std::find_if(m_treeList.begin(), m_treeList.end(), [t](const GameTree& tree) -> bool
			{
				return tree.m_treeID == t;
			});

		if (iter != m_treeList.end()) 
		{
			m_treeList.erase(iter);
			iter->ClearAllNodes();
		}
		else   // The tree should be inside the list. if its not inside, we will log a message
		{
			GE::Debug::ErrorLogger::GetInstance().LogWarning("Somehow you have an ID of a tree that doesnt exist, please check the code", false);
		}
	}

	for (Entity entity : GetUpdatableEntities()) { 
		GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
		GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
		auto iter = std::find_if(delTrees.begin(), delTrees.end(), [enemyAIComp](TreeID treeID) -> bool
			{
				return treeID == enemyAIComp->m_treeID;
			});
		if (iter != delTrees.end()) 
		{
			enemyAIComp->m_treeID = GE::Component::ghostTreeID;     // If any entity was using the deleted tree, we set its ID to an invalid ID
			enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.clear(); // Clear the cache since the tree is gone
		}
	}

	delTrees.clear(); //Clear the list after updating the enemy system's list
}


void EnemySystem::UpdateTreeList()
{
	std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
	for (TreeTemplate& tt : tempTreeList)
	{
		if (tt.m_treeChanged == true)
		{
			std::vector<GameTree>::iterator iter = std::find_if(m_treeList.begin(), m_treeList.end(), [tt](GameTree& tree) -> bool
				{
					return tree.m_treeID == tt.m_treeTempID;
				});

			if (iter != m_treeList.end()) //If we already have that tree in our list, we will just swap
			{
				GameTree newGamTree = GenerateGameTree(tt);
				std::swap(iter->m_nodeList, newGamTree.m_nodeList);

			}
			else // We got a completely new tree, we will just add it into the system
			{
				AddGameTree(tt);
			}

			for (Entity entity : GetUpdatableEntities()) {
				GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
				GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
				if (enemyAIComp->m_treeID == tt.m_treeTempID)
				{
					enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.clear(); // Clear the cache since the tree is updated
				}
			}

			tt.m_treeChanged = false;
		}
	}


}

