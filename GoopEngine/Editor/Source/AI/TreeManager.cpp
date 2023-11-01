#include <pch.h>
#include <AI/TreeManager.h>

using namespace GE;
using namespace AI;

void TreeManager::Init()
{
  //Json code to deserialize

  GE::AI::NodeTemplate node0{ ROOT_NODE,		 0, {1},      "RootNode", {50.0, 80.0} };						   //0
  GE::AI::NodeTemplate node1{ COMPOSITE_NODE,0, {2,5},  "SelectorNode", {230.0, 80.0} };					   //1
  GE::AI::NodeTemplate node2{ COMPOSITE_NODE,1, {3,4},		"SequenceNode", {410.0, 20.0} };				     //2
  GE::AI::NodeTemplate node3{ LEAF_NODE,		 2, {0},      "IsOutsideAttackRange", {590.0, 0.0} };   //3
  GE::AI::NodeTemplate node4{ LEAF_NODE,		 2, {0},      "MoveToPlayer", {590.0, 100.0} };           //4
  GE::AI::NodeTemplate node5{ COMPOSITE_NODE,1, {6,7},		"SequenceNode", {410.0, 170.0} };				     //5
  GE::AI::NodeTemplate node6{ LEAF_NODE,		 5, {0},      "IsWithinPlayerRange", {590.0, 200.0} };    //6
  GE::AI::NodeTemplate node7{ LEAF_NODE,		 5, {0},      "RunFromPlayer", {590.0, 300.0} };          //7

  std::vector<GE::AI::NodeTemplate> tree{ node0, node1,node2, node3 ,node4, node5,node6, node7 };
  GE::AI::TreeTemplate newTemp{ tree,"WormTree",0 };
  m_treeTempList.push_back(std::make_pair(false, newTemp));
}

void TreeManager::ShutDown()
{
  //Json code to serialize
}

std::vector<std::pair<bool, TreeTemplate>>& TreeManager::GetTreeList()
{
	return m_treeTempList;
}

void TreeManager::UpdateTreeList(TreeTemplate& treeTemp)
{
  TreeID treeID = treeTemp.m_treeTempID;
  auto iter = std::find_if(m_treeTempList.begin(), m_treeTempList.end(), [treeID](const std::pair<bool, TreeTemplate>& tree) -> bool
  {
    return tree.second.m_treeTempID == treeID;
  });

  if (iter != m_treeTempList.end())   // If the tree Template already exist in the list, we just update with the new one
  {
    std::swap(iter->second, treeTemp);
    iter->first = true;
  }
  else // If the tree Template does not exist, we will add it into list
  {
    m_treeTempList.push_back(std::make_pair(true, treeTemp));
  }
  m_listChanged = true;
}

bool TreeManager::isTreeUpdated()
{
  return m_listChanged;
}

void TreeManager::SetTreeBool(bool cond)
{
  m_listChanged = cond;
}
