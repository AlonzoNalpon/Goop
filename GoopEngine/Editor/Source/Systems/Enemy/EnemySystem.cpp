/*!************************************************************************
\file EnemySystem.cpp
\author Han Qin Ding
\date  26-November-2023

\brief
This file contains functions for the Enemy System Class
The enemy system is in charge of running all the AI behaviours of the enemies.
Each behaviour tree can be used by multiple enemies.
Instead of created 1 copy of the tree for each enemy, we will only have 1 copy of the tree at all times.
Each enemy will have a tree cache. This cache will be used to keep track of the enemy's progress within the behaivour tree.
Each enemy will only resolve 1 leaf node per frame, their cache will help the system determin which node the enemy will traverse
to in the current frame.

**************************************************************************/
#include <pch.h>
#include <Systems/Enemy/EnemySystem.h>
#include <AssetManager/AssetManager.h>
#include <ImNode/NodeEditor.h>

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
	//Get all the trees
	const std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
	for (size_t i{ 0 }; i < tempTreeList.size(); ++i)
	{
		AddGameTree(tempTreeList[i]);
	}
	// Set the first tree as the current tree
	m_currentTree = (m_treeList.size() != 0) ? &(m_treeList[0]) : nullptr;
}

void EnemySystem::FixedUpdate()
{
	
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	// Only update if there are actual trees
	if (m_treeList.size() != 0)
	{
		for (Entity entity : GetUpdatableEntities()) {
			GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
			GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
			GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
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
				mono_runtime_invoke(onUpdateMethod, m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classInst, arg.data(), nullptr);
				//PLEASE UNCOMMENT THIS IF YOU WANT TO SEE THE TREE CACHE OF THE ENEMY AT THE END OF EVRY FRAME
				//PrintNodeCache(enemyAIComp->m_enemyTreeCache.m_nodeCacheStack);
			}
			else
			{
				GE::Debug::ErrorLogger::GetInstance().LogWarning("Your tree has no root node, please a root node to the start of the tree", false);
			}
		}
	}

	// we have a newly updated tree, need to update the in game tree
	if (GE::AI::TreeManager::GetInstance().isTreeUpdated())
	{
		std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
		std::vector<bool>& tempTreeCondList = GE::AI::TreeManager::GetInstance().GetTreeCondList();

		for (size_t i{ 0 }; i < tempTreeList.size(); ++i)
		{
			// Only swap the trees that are newly updated
			if (tempTreeCondList[i])
			{
				if (tempTreeList[i].m_treeTempID >= static_cast<unsigned>(m_treeList.size()))
				{
					AddGameTree(tempTreeList[i]);
				}
				else
				{
					GameTree newGamTree = GenerateGameTree(tempTreeList[i]);
					for (GameTree& gameTree : m_treeList)
					{
						if (gameTree.m_treeID == tempTreeList[i].m_treeTempID)
						{
							std::swap(gameTree.m_nodeList, newGamTree.m_nodeList);
						}
					}

					GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
					for (Entity entity : GetUpdatableEntities())
					{
						GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
						if (enemyAIComp->m_treeID == tempTreeList[i].m_treeTempID)
						{
							enemyAIComp->RefreshCache();
						}
					}
				}
				tempTreeCondList[i] = false;
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

	//Loop through each node in the tree and create a new GameNode
	for (size_t i{ 0 }; i < tree.size(); ++i)
	{
		NodeID ownID = static_cast<unsigned int>(i);
		NodeID parentID = tree[i].m_parentNode;
		unsigned int listSize = static_cast<unsigned int>(tree[i].m_childrenNode.size());

		MonoArray* result = mono_array_new(mono_domain_get(), mono_get_uint32_class(), listSize);
		for (unsigned int j = 0; j < listSize; j++) {
			mono_array_set(result, unsigned int, j, tree[i].m_childrenNode[j]);
		}
		std::vector<void*> arg{ &ownID, &parentID, result, &listSize };
		GE::MONO::ScriptInstance scriptInst = GE::MONO::ScriptInstance(tree[i].m_scriptName.c_str(), arg);
		newGamTree.m_nodeList.push_back(GameNode(tree[i].m_nodeType, scriptInst));
	}
	return newGamTree;
}

void EnemySystem::UseTree(TreeID treeID, unsigned int entityID)
{
	m_currentEntityID = entityID;
	m_currentTreeID = treeID;
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
	mono_runtime_invoke(onUpdateMethod, m_currentTree->m_nodeList[childNodeID].m_script.m_classInst, arg.data(), nullptr);


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
			mono_runtime_invoke(onUpdateMethod, m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classInst, arg.data(), nullptr);
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

//void EnemySystem::SetPlayerID()
//{
//	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
//	std::set<GE::ECS::Entity> entityList = ecs->GetEntities();
//	m_playerExist = false;
//	for (GE::ECS::Entity e : entityList)
//	{
//		if (ecs->GetEntityName(e) == "Player")
//		{
//			m_playerExist = true;
//			m_playerID = e;
//			break;
//		}
//	}
//}