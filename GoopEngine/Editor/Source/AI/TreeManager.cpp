#include <pch.h>
#include <AI/TreeManager.h>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include "../AssetManager/AssetManager.h"

using namespace GE;
using namespace AI;

void TreeManager::Init()
{
  try
  {
    Assets::AssetManager& assetManager{ Assets::AssetManager::GetInstance() };
    m_treeTempList = std::move(GE::Serialization::Deserializer::DeserializeTrees(assetManager.GetConfigData<std::string>("BehaviourTree file").c_str()));
    // //Bool to represent if its changedm_treeTempCond.resize(m_treeTempList.size(), false);

  }
  catch (...)
  {
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Error deserializing tree templates"));
  }
}

void TreeManager::ShutDown()
{
  Assets::AssetManager& assetManager{ Assets::AssetManager::GetInstance() };
  GE::Serialization::Serializer::SerializeAny(assetManager.GetConfigData<std::string>("BehaviourTree file").c_str(), m_treeTempList);
  std::cout << "shutdown\n";
}

std::vector<TreeTemplate>& TreeManager::GetTreeList()
{
  return m_treeTempList;
}

std::vector<TreeID>& TreeManager::GetDelTreeList()
{
  return m_deletedTress;
}


void TreeManager::UpdateTreeList(TreeTemplate& treeTemp)
{
  TreeID treeID = treeTemp.m_treeTempID;
  auto iter = std::find_if(m_treeTempList.begin(), m_treeTempList.end(), [treeID](const TreeTemplate& tree) -> bool
    {
      return tree.m_treeTempID == treeID;
    });

  if (iter != m_treeTempList.end())   // If the tree Template already exist in the list, we just update with the new one
  {
    std::cout << "replace\n";
    std::swap(*(iter), treeTemp);
    //m_treeTempCond[(iter - m_treeTempList.begin())] = true;
  }
  else // If the tree Template does not exist, we will add it into list
  {
    std::cout << "ADDNEW :: "  << treeTemp.m_treeTempID << "\n";
    m_treeTempList.push_back(treeTemp);
    //m_treeTempCond.push_back(true);
  }
  m_listChanged = true;

}

void TreeManager::DeleteTree(unsigned pos)
{
  m_deletedTress.push_back(m_treeTempList[pos].m_treeTempID);
  m_treeTempList.erase(m_treeTempList.begin() + pos);
  //m_treeTempCond.erase(m_treeTempCond.begin() + pos);
  m_listChanged = true;
  //std::cout << m_treeTempList.size() << "::" << m_treeTempCond.size() << "\n";
}

bool TreeManager::isTreeUpdated()
{
  return m_listChanged;
}

void TreeManager::SetTreeBool(bool cond)
{
  m_listChanged = cond;
}
