#include <pch.h>
#include <Systems/Enemy/EnemySystem.h>


using namespace GE;
using namespace AI;
using namespace ECS;
using namespace Systems;



unsigned int EnemySystem::m_currentEntityID;
unsigned int EnemySystem::m_currentTreeID;
unsigned int EnemySystem::m_playerID;
std::vector<GE::AI::Tree> EnemySystem::m_treeList;


void EnemySystem::Update()
{

	for (Entity entity : GetUpdatableEntities()) {
		GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
		GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(entity);
		GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
		UseTree(enemyAIComp->m_enemyTreeCache.m_treeID, entity);
		std::cout << "ENTITY :" << entity << " using tree\n";
		//std::cout << m_currentEntityID << "lestsog\n";

		MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_treeList[m_currentTreeID][enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst), "OnUpdate", 2);
		double dt = fpsControl->GetDeltaTime();
		std::vector<void*> arg{ &m_currentEntityID, &dt };
		mono_runtime_invoke(onUpdate, m_treeList[m_currentTreeID][enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst, arg.data(), nullptr);
		PrintNodeCache(enemyAIComp->m_enemyTreeCache.m_nodeCacheStack);
		std::cout << "END FOR THIS FRAME\n\n";
	}
}


void EnemySystem::UseTree(TreeID treeID, unsigned int entityID)
{
	m_currentEntityID = entityID;
	m_currentTreeID = treeID;
}
//
void EnemySystem::InitTree(const std::vector<Tree>& treeList)
{
	m_treeList = treeList;
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

	MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_treeList[m_currentTreeID][childNodeID].m_script.m_classObjInst), "OnUpdate", 2);
	double dt = fpsControl->GetDeltaTime();
	std::vector<void*> arg{ &m_currentEntityID, &dt };
	mono_runtime_invoke(onUpdate, m_treeList[m_currentTreeID][childNodeID].m_script.m_classObjInst, arg.data(), nullptr);
}



void EnemySystem::JumpToParent()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);

	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.pop_front();

	if (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_NodeResult == STATE_WAITING)
	{
		MonoMethod* onUpdate = mono_class_get_method_from_name(mono_object_get_class(m_treeList[m_currentTreeID][enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst), "ReturnFromChild", 1);
		std::vector<void*> arg{ &m_currentEntityID };
		mono_runtime_invoke(onUpdate, m_treeList[m_currentTreeID][enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_nodeID].m_script.m_classObjInst, arg.data(), nullptr);
	}
}

void EnemySystem::ResetNode()
{
	GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
	GE::Component::EnemyAI* enemyAIComp = ecs->GetComponent<GE::Component::EnemyAI>(m_currentEntityID);

	while (enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.size() > 1)
	{
		enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.pop_front();
	}

	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_childIndex = 0;
	enemyAIComp->m_enemyTreeCache.m_nodeCacheStack.front().m_NodeResult = STATE_NEW;
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
