#include <pch.h>
#include <Systems/Enemy/EnemySystem.h>
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


void EnemySystem::InitTree()
{
	std::cout << "INIT TREE\n";
	//Get all the trees
	const std::vector<std::pair<bool, TreeTemplate>>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
	for (const std::pair<bool, TreeTemplate>& treeTemp : tempTreeList)
	{
		std::cout << treeTemp.second.m_treeName << "ADDED\n";
		AddGameTree(treeTemp.second);
	}

	// Set the first tree as the current tree
	m_currentTree = (m_treeList.size() != 0) ? &(m_treeList[0]) : nullptr;
}

void EnemySystem::FixedUpdate()
{
	// Only update if there are actual trees
	if (m_treeList.size() != 0)
	{
		for (Entity entity : GetUpdatableEntities()) {

			GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
			GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
			GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
			UseTree(enemyAIComp->m_enemyTreeCache.m_treeID, entity);
			
			MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst), "OnUpdate", 2);
			double dt = fpsControl->GetFixedDeltaTime();
			std::vector<void*> arg{ &m_currentEntityID, &dt };
			mono_runtime_invoke(onUpdate, m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst, arg.data(), nullptr);
		}
	}

	// we have a newly updated tree, need to update the in game tree
	if (GE::AI::TreeManager::GetInstance().isTreeUpdated())
	{
		GE::AI::TreeManager* tm = &(GE::AI::TreeManager::GetInstance());
		for ( std::pair<bool, TreeTemplate>& treeTemp : tm->GetTreeList())
		{
			// Only swap the trees that are newly updated
			if (treeTemp.first)
			{
				std::cout << "SWAP\n";
				if (treeTemp.second.m_treeTempID >= static_cast<unsigned>(m_treeList.size()))
				{
					AddGameTree(treeTemp.second);
				}
				else
				{
					GameTree newGamTree = GenerateGameTree(treeTemp.second);
					for (GameTree& gameTree : m_treeList)
					{
						if (gameTree.m_treeID == treeTemp.second.m_treeTempID)
						{
							std::swap(gameTree.m_nodeList, newGamTree.m_nodeList);
						}
					}

					GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
					for (Entity entity : GetUpdatableEntities())
					{
						GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
						if (enemyAIComp->m_enemyTreeCache.m_treeID == treeTemp.second.m_treeTempID)
						{
							enemyAIComp->RefreshCache();
						}
					}
				}
				treeTemp.first = false;
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
	GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
	const std::vector<NodeTemplate>& tree = treeTemp.m_tree;

	GameTree newGamTree{ {},treeTemp.m_treeTempID };

	// Loop through each node in the tree and create a new GameNode
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
		newGamTree.m_nodeList.push_back(GameNode(GE::MONO::Script(scriptMan->InstantiateClass("GoopScripts.AI", tree[i].m_scriptName.c_str(), arg))));
	}
	return newGamTree;
}

void EnemySystem::UseTree(TreeID treeID, unsigned int entityID)
{
	m_currentEntityID = entityID;
	m_currentTreeID = treeID;
}

void EnemySystem::SetPlayerID(unsigned int playerID)
{
	m_playerID = playerID;
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
		n.m_NodeResult = (n.m_nodeID == currID)?static_cast<GE::AI::NODE_STATES>(result): n.m_NodeResult;
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

	MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_currentTree->m_nodeList[childNodeID].m_script.m_classObjInst), "OnUpdate", 2);
	double dt = fpsControl->GetDeltaTime();
	std::vector<void*> arg{ &m_currentEntityID, &dt };

	mono_runtime_invoke(onUpdate, m_currentTree->m_nodeList[childNodeID].m_script.m_classObjInst, arg.data(), nullptr);
}



void EnemySystem::JumpToParent()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);

	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.pop_front();

	if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_NodeResult == STATE_WAITING)
	{
		MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst), "ReturnFromChild", 1);
		std::vector<void*> arg{ &m_currentEntityID };

		mono_runtime_invoke(onUpdate, m_currentTree->m_nodeList[enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst, arg.data(), nullptr);
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


void GE::Systems::PrintNodeCache(const std::deque<GE::AI::NodeCache>& temp)
{

	std::cout << "STACK Detail:\n";
	std::cout << "-----------------------\n";
	for(const GE::AI::NodeCache& n:temp)
	{
		std::cout << n.m_childIndex << ":" << GE::AI::nodestateNames[n.m_NodeResult] << "\n";
	}
	std::cout << "-----------------------\n\n";
}
