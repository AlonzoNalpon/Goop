#include <pch.h>
#include <AI/Tree.h>
#include <ScriptEngine/ScriptManager.h>



//
//GE::AI::Tree GE::AI::CreateTree(const std::vector<GE::AI::NodeTemplate>& treeTemp)
//{
//	GE::AI::Tree newTree{ treeTemp., treeTemp.size() };
//	GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
//
//	for (size_t i{0};i<treeTemp.size();++i)
//	{
//		NodeID ownID = static_cast<unsigned int>(i);
//		NodeID copy = treeTemp[i].m_parentNode;
//		unsigned int listSize = static_cast<unsigned int>(treeTemp[i].m_childrenNode.size());
//
//		MonoArray* result = mono_array_new(mono_domain_get(), mono_get_uint32_class(), listSize);
//		for (unsigned int j = 0; j < listSize; j++) {
//			mono_array_set(result, unsigned int, j, treeTemp[i].m_childrenNode[j]);
//		}
//
//
//		std::vector<void*> arg{ &ownID, &copy, result, &listSize };
//		newTree[i].m_script = GE::MONO::Script(scriptMan->InstantiateClass("GoopScripts.AI", treeTemp[i].m_scriptName.c_str(), arg));
//	}
//
//	return newTree;
//}


//GE::AI::TreeCache::TreeCache() 
//{
//	m_nodeCacheStack = std::deque<GE::AI::NodeCache>();
//	m_nodeCacheStack.push_front((GE::AI::NodeCache(0, 0, GE::AI::NODE_STATES::STATE_NEW)));
//	std::cout << m_nodeCacheStack.size() << "\n";
//}
