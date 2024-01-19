#include <pch.h>
#ifndef IMGUI_DISABLE
#include <ImNode/NodeEditor.h>
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "../AssetManager/AssetManager.h"



void GE::AI::NodeEditor::NodeEditorInit()
{
  // Get all the names of the scripts available
  Assets::AssetManager& assetManager{ Assets::AssetManager::GetInstance() };
  std::ifstream file(assetManager.GetConfigData<std::string>("AIScriptNames").c_str());
  if (!file.good())
  {
    throw Debug::Exception<GE::AI::NodeEditor>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to open GoopScripts AI folder: "+ assetManager.GetConfigData<std::string>("AIScriptNames")));
  }

  std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  rapidjson::Document document;
  document.Parse(jsonString.c_str());

  if (document.HasParseError()) {
    throw Debug::Exception<GE::AI::NodeEditor>(Debug::LEVEL_CRITICAL, ErrMsg("Parsing error for: " + assetManager.GetConfigData<std::string>("AIScriptNames")));
  }

  if (document.HasMember("Enemy") && document["Enemy"].IsArray()) {
    const rapidjson::Value& scriptNames = document["Enemy"];

    // Iterate through the array and get the script names
    for (rapidjson::SizeType i = 0; i < scriptNames.Size(); i++) {
      m_allScriptNames.push_back(scriptNames[i].GetString());
    }
  }
  else {
    throw Debug::Exception<GE::AI::NodeEditor>(Debug::LEVEL_CRITICAL, ErrMsg("Json File not structed properly: " + assetManager.GetConfigData<std::string>("ScriptNames")));
  }

  //Create the node editor context and turn on any settings
  ImNodes::CreateContext();
  ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
  ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

  //Get all the trees
  const std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();
  m_maxTreeID = 0;

  for (size_t i{ 0 }; i < tempTreeList.size(); ++i)
  {
    AddDisplayTree(tempTreeList[i]);
  }

  m_currentTree = (m_treeList.size() != 0) ? &(m_treeList[0].first) : nullptr;
  m_currentLinkList = (m_treeList.size() != 0) ? &(m_treeList[0].second) : nullptr;
  m_currentTreeInd = (m_treeList.size() != 0) ? 0 : static_cast<unsigned>(EMPTYID);
  ++m_maxTreeID;
}

void  GE::AI::NodeEditor::NodeEditorShow()
{

  ImNodes::BeginNodeEditor();
  DisplayTreeOption();

  if (m_currentTree != nullptr)
  {
    m_displayPopup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1);
    ShowTree();
  }
  ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
  ImNodes::MiniMap();
  ImNodes::EndNodeEditor();

  if (m_currentTree != nullptr)
  {
    // Check if any new links have been made
    NodeLinks createdLink{};
    if (ImNodes::IsLinkCreated(&createdLink.m_pinIDs.first, &createdLink.m_pinIDs.second))
    {
      m_currentTree->m_changedData = true;
      createdLink.m_linkID = m_currentLinkList->m_maxLinkID++;
      m_currentLinkList->m_linkList.push_back(createdLink);
    }

    //Check if any link has been destroyed
    int link_id = 100;
    if (ImNodes::IsLinkDestroyed(&link_id))
    {
      m_currentTree->m_changedData = true;
      std::vector<NodeLinks>& currLinks = m_currentLinkList->m_linkList;
      auto iter = std::find_if(currLinks.begin(), currLinks.end(), [link_id](const NodeLinks& link) -> bool
        {
          return link.m_linkID == link_id;
        });
      if (iter != currLinks.end())
      {
        currLinks.erase(iter);
      }
    }

    //Update the nodes new position if they have been moved 
    int numSelectedNodes = ImNodes::NumSelectedNodes();
    std::vector<int> selectedNodes(numSelectedNodes);

    if (numSelectedNodes > 0)
    {

      ImNodes::GetSelectedNodes(selectedNodes.data());
      for (const int nID : selectedNodes)
      {
        m_currentTree->m_changedPos = (m_currentTree->m_displayNodes[nID].m_pos.x != ImNodes::GetNodeGridSpacePos(nID).x || m_currentTree->m_displayNodes[nID].m_pos.y != ImNodes::GetNodeGridSpacePos(nID).y)
          ? true : m_currentTree->m_changedPos;
        m_currentTree->m_displayNodes[nID].m_pos = ImNodes::GetNodeGridSpacePos(nID);
      }
    }

    if (m_displayPopup)
    {

      if (numSelectedNodes == 0)
      {
        ImGui::OpenPopup("add node");
      }
      else
      {
        m_selectedNodeInd = selectedNodes[0];
        ImGui::OpenPopup("edit node");
      }
    }

    DisplayPopup();

    // Update the Tree Manager if user made some changes to the tree's data.
    // We do not update if the node's position were changed. We will do that when we shutdown
    if (m_currentTree->m_changedData == true)
    {
      UpdateNewTree();
    }
  }
}



void  GE::AI::NodeEditor::NodeEditorShutdown()
{
  ImNodes::DestroyContext();
}

void GE::AI::NodeEditor::ShowTree()
{
  int currDiplayNodeID{ 0 };

  //Draw out all the nodes of the current tree
  for (GE::AI::DisplayNode& dn : m_currentTree->m_displayNodes)
  {
    //Set the title color of the Node based on the node Type
    ImNodesStyle& style = ImNodes::GetStyle();
    const NodeRGBA& titleCol = titleColors[static_cast<int>(dn.m_nodeType)];
    style.Colors[ImNodesCol_TitleBar] = IM_COL32(titleCol.r, titleCol.g, titleCol.b, titleCol.a);

    ////set the node's background color based on whether its running 
    //std::vector<unsigned int>::iterator it = std::find(m_currentRunningNode.begin(), m_currentRunningNode.end(), static_cast<unsigned int>(currDiplayNodeID));
    //const NodeRGBA& bgCol = nodeBgColors[(it == m_currentRunningNode.end())];
    //style.Colors[ImNodesCol_NodeBackground] = IM_COL32(bgCol.r, bgCol.g, bgCol.b, bgCol.a);
    ImNodes::BeginNode(currDiplayNodeID);


    //Set the title name
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(nodeTitles[static_cast<int>(dn.m_nodeType)].c_str());
    ImNodes::EndNodeTitleBar();

    //Draw the Node
    ImGui::Dummy(nodeDispSize);
    ImNodes::SetNodeGridSpacePos(currDiplayNodeID, ImVec2(static_cast<float>(dn.m_pos.x), static_cast<float>(dn.m_pos.y)));

    // Display the script of the node
    ImGui::TextUnformatted("Script: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(100.0f);


    if (ImGui::BeginCombo("##combo", dn.m_scriptName.c_str(), ImGuiComboFlags_NoArrowButton))
    {
      for (const std::string& s : m_allScriptNames)
      {
        bool is_selected = (dn.m_scriptName.c_str() == s);
        if (ImGui::Selectable(s.c_str(), is_selected))
        {
          m_currentTree->m_changedData = (dn.m_scriptName != s) ? true : m_currentTree->m_changedData;
          dn.m_scriptName = s;
        }
        if (is_selected)
        {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::NewLine();


    //Set the input pin
    if (dn.m_nodeType != ROOT_NODE) //Root Node has not input Node
    {
      ImNodes::BeginInputAttribute(dn.m_intputPinID);
      ImGui::TextUnformatted("Parent");
      ImNodes::EndInputAttribute();
    }


    //Set the output pin
    if (dn.m_nodeType != LEAF_NODE) //Leaf node has no outputNode
    {
      ImNodes::BeginOutputAttribute(dn.m_outputPinID);
      ImGui::Indent(130);
      ImGui::TextUnformatted("Child");
      ImNodes::EndInputAttribute();
    }
    ImNodes::EndNode();
    ++currDiplayNodeID;
  }


  //Set the links
  for (const NodeLinks& l : m_currentLinkList->m_linkList)
  {
    ImNodes::Link(l.m_linkID, l.m_pinIDs.first, l.m_pinIDs.second); // <start,end>, <output, intput>
  }
}
void GE::AI::NodeEditor::DisplayTreeOption()
{
  //ImVec4 originalFrameBg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
  //ImVec4 originalFrameBgHovered = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered);
  //ImVec4 originalbuttonBg = ImGui::GetStyleColorVec4(ImGuiCol_Button);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f));

  //ImVec4 customColor = ImVec4(0.f, 0.f, 0.f, 1.0f);
  //ImVec4 customColor2 = ImVec4(0.13f, 0.55f, 0.34f, 1.0f);
  //ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = customColor;
  //ImGui::GetStyle().Colors[ImGuiCol_Button] = customColor;
  //ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = customColor2;


  float itemWidth = 100.0f; // Set your desired item width
 
  
 
 
  if (m_treeList.size() > 1)
  {
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (itemWidth + 10.0f) * 3.f);
    ImGui::SetNextItemWidth(100.0f); // Adjust the width as needed
    if (ImGui::Button("Delete Tree")) {
    // Handle button click
  
      m_treeList.erase(m_treeList.begin() + m_currentTreeInd);
      GE::AI::TreeManager::GetInstance().DeleteTree(m_currentTreeInd);
      m_currentTreeInd = static_cast<unsigned>(m_treeList.size()-1);
      m_currentTree = &(m_treeList[m_currentTreeInd]).first;
      m_currentLinkList = &(m_treeList[m_currentTreeInd]).second;
      m_selectedNodeInd = 0;
      ImNodes::ClearNodeSelection();
      ImNodes::ClearLinkSelection();
    }
    ImGui::SameLine();
  }
  else
  {
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (itemWidth + 10.0f) * 2.f);
  }

  // Use SameLine to keep the button and the dropdown on the same line

  // Set your desired color

  ImGui::SetNextItemWidth(100.0f); // Adjust the width as needed
  if (ImGui::Button("Add New Tree")) {
    // Handle button click
    DisplayTree newTree{};
    NodeLinkList newLink{};
    m_treeList.push_back(std::make_pair(newTree, newLink));
    m_currentTreeInd = static_cast<unsigned>(m_treeList.size() - 1);
    m_currentTree = &(m_treeList[m_currentTreeInd]).first;
    m_currentLinkList = &(m_treeList[m_currentTreeInd]).second;
    m_selectedNodeInd = 0;
    m_displayPopup = false;
    m_currentTree->m_changedData = true;
    m_currentTree->m_treeName = "newTree" + std::to_string(m_maxTreeID);
    m_currentTree->m_treeID = m_maxTreeID++;
    ImNodes::ClearNodeSelection();
    ImNodes::ClearLinkSelection();
  }

  // Use SameLine to keep the button and the dropdown on the same line
  ImGui::SameLine();
    // Set your desired color



  ImGui::SetNextItemWidth(100.0f); // Adjust the width as needed
  // Create a dropdown
  if (ImGui::BeginCombo("##TreeList", m_treeList[m_currentTreeInd].first.m_treeName.c_str())) {
    for (size_t i{0};i<m_treeList.size();i++)
    {
      bool is_selected = (i != m_currentTreeInd);
      if (i != m_currentTreeInd)
      {
        if (ImGui::Selectable(m_treeList[i].first.m_treeName.c_str(),is_selected))
        {
          m_currentTreeInd = static_cast<unsigned>(i);
          m_currentTree = &(m_treeList[m_currentTreeInd]).first;
          m_currentLinkList = &(m_treeList[m_currentTreeInd]).second;
          m_selectedNodeInd = 0;
          m_displayPopup = false;
          ImNodes::ClearNodeSelection();
          ImNodes::ClearLinkSelection();
        }
      }
      if (is_selected)
      {
        ImGui::SetItemDefaultFocus();
      }
    }
    


    // End the dropdown
    ImGui::EndCombo();
  }

  //ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = originalFrameBg;
  //ImGui::GetStyle().Colors[ImGuiCol_Button] = originalbuttonBg;
  //ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = originalFrameBgHovered;
  ImGui::PopStyleVar();
}


void GE::AI::NodeEditor::DisplayPopup()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));

  if (ImGui::BeginPopup("add node"))
  {
    ImVec2 nodePos = ImGui::GetMousePos();
    for (int i{ 0 }; i < static_cast<int>(NODE_TYPE_COUNT); ++i)
    {
      if (ImGui::MenuItem(addNodeOption[i].c_str()))
      {
        DisplayNode dispNode{};
        dispNode.m_scriptName = std::string();
        dispNode.m_intputPinID = m_currentTree->m_maxPinID++;
        dispNode.m_outputPinID = m_currentTree->m_maxPinID++;
        dispNode.m_NodeID = m_currentTree->m_maxNodeID++;
        dispNode.m_nodeType = static_cast<GE::AI::NODE_TYPE>(i);
        ImNodes::SetNodeScreenSpacePos(static_cast<int>(m_currentTree->m_displayNodes.size()), nodePos);
        dispNode.m_pos = ImNodes::GetNodeGridSpacePos(static_cast<int>(m_currentTree->m_displayNodes.size()));

        m_currentTree->m_displayNodes.push_back(dispNode);
      }
    }
    ImGui::EndPopup();
  }

  else if (ImGui::BeginPopup("edit node"))
  {
    ImVec2 nodePos = ImGui::GetMousePos();
    for (int i{ 0 }; i < static_cast<int>(editNodeOption.size()); ++i)
    {
      if (i != static_cast<int>(m_currentTree->m_displayNodes[m_selectedNodeInd].m_nodeType))
      {
        if (ImGui::MenuItem(editNodeOption[i].c_str()))
        {

          //Check if the user decides to delete the node
          if (i == static_cast<int>(editNodeOption.size() - 1))
          {
            //Remove the link connecting to the input pin of the node
            int inputID = m_currentTree->m_displayNodes[m_selectedNodeInd].m_intputPinID;
            int outputID = m_currentTree->m_displayNodes[m_selectedNodeInd].m_outputPinID;

            m_currentLinkList->m_linkList.erase(std::remove_if(m_currentLinkList->m_linkList.begin(), m_currentLinkList->m_linkList.end(),
              [inputID, outputID](const NodeLinks& l) {
                return (l.m_pinIDs.second == inputID || l.m_pinIDs.first == outputID);
              }),
              m_currentLinkList->m_linkList.end()
                );

            ImNodes::ClearNodeSelection(m_selectedNodeInd);
            m_currentTree->m_displayNodes.erase(m_currentTree->m_displayNodes.begin() + m_selectedNodeInd);
          }
          else
          {
            //Remove the link connecting to the input pin of the node
            int inputID = m_currentTree->m_displayNodes[m_selectedNodeInd].m_intputPinID;
            int outputID = m_currentTree->m_displayNodes[m_selectedNodeInd].m_outputPinID;

            m_currentLinkList->m_linkList.erase(std::remove_if(m_currentLinkList->m_linkList.begin(), m_currentLinkList->m_linkList.end(),
              [inputID, outputID](const NodeLinks& l) {
                return (l.m_pinIDs.second == inputID || l.m_pinIDs.first == outputID);
              }),
              m_currentLinkList->m_linkList.end()
                );
            m_currentTree->m_displayNodes[m_selectedNodeInd].m_nodeType = static_cast<NODE_TYPE>(i);
          }
          m_currentTree->m_changedData = true;
        }
      }
    }
    ImGui::EndPopup();
  }
  ImGui::PopStyleVar();
}

void GE::AI::NodeEditor::UpdateNewTree()
{
  std::cout << "NEWTREEUPDATE\n";
  std::cout << "----------------------------------\n";
  //Create the newTempTree
  TreeTemplate treeTemp{};
  treeTemp.m_treeName = m_currentTree->m_treeName;
  treeTemp.m_treeTempID = m_currentTree->m_treeID;
  bool hasEmptyNode{ false };
  // PrintDetails();
  int currID{ 0 };
  for (const DisplayNode& dispNode : m_currentTree->m_displayNodes)
  {
    if (dispNode.m_scriptName == std::string())
    {
      hasEmptyNode = true;
      break;
    }
    std::cout << dispNode.m_scriptName << "\n";
    NodeTemplate nodeTemp{ dispNode.m_nodeType,0,{},dispNode.m_scriptName,{dispNode.m_pos.x, dispNode.m_pos.y} };
    std::vector<int> childPinID{};
    int parentPinID{};

    //Get the pinID of the Parent/Child
    for (const NodeLinks& link : m_currentLinkList->m_linkList)
    {
      parentPinID = (link.m_pinIDs.second == dispNode.m_intputPinID) ? link.m_pinIDs.first : parentPinID;
      if (link.m_pinIDs.first == dispNode.m_outputPinID)
      {
        childPinID.push_back(link.m_pinIDs.second);
      }
    }
    //Using the pinID of the Parent/Child to find the NodeID of the parent/child and push it into the node
    for (size_t i{ 0 }; i < m_currentTree->m_displayNodes.size(); ++i)
    {
      nodeTemp.m_parentNode = (m_currentTree->m_displayNodes[i].m_outputPinID == parentPinID) ? static_cast<NodeID>(i) : nodeTemp.m_parentNode;
    }

    for (size_t j{ 0 }; j < childPinID.size(); ++j)
    {
      for (size_t i{ 0 }; i < m_currentTree->m_displayNodes.size(); ++i)
      {
        if ((m_currentTree->m_displayNodes[i].m_intputPinID == childPinID[j]))
        {
          nodeTemp.m_childrenNode.push_back(static_cast<NodeID>(i));
          break;
        }
      }
    }

    ++currID;
    treeTemp.m_tree.push_back(nodeTemp);

  }
  //Sent the updated tree to Tree Manager
  if (!hasEmptyNode)
  {
    GE::AI::TreeManager::GetInstance().UpdateTreeList(treeTemp);
  }
  m_currentTree->m_changedData = false;

}

void GE::AI::NodeEditor::AddDisplayTree(const TreeTemplate& tree)
{
  std::pair<DisplayTree, NodeLinkList> newTree{};
  DisplayTree& displayTree{ newTree.first };
  NodeLinkList& links{ newTree.second };


  //Set up the display tree
  int currNodeID{ 0 };
  for (const GE::AI::NodeTemplate& nt : tree.m_tree)
  {
    // Set the input pin to link with parent
    int inputPinID = displayTree.m_maxPinID++; //Root node has no parent
    int outputPinID = displayTree.m_maxPinID++; //Leaf node has no child
    DisplayNode newDisplayNode = DisplayNode(nt.m_scriptName, outputPinID, inputPinID, currNodeID, nt.m_nodeType, ImVec2(nt.m_pos.x, nt.m_pos.y));
    displayTree.m_displayNodes.push_back(newDisplayNode);
    ++currNodeID;
  }
  displayTree.m_treeName = tree.m_treeName;
  displayTree.m_treeID = tree.m_treeTempID;
  displayTree.m_maxNodeID = currNodeID;
  m_maxTreeID = (displayTree.m_treeID >= m_maxTreeID) ? displayTree.m_treeID : m_maxTreeID;


  //Set up the links
  int newLinkID{ 0 };
  for (size_t i{ 0 }; i < displayTree.m_displayNodes.size(); ++i)
  {
    if (displayTree.m_displayNodes[i].m_nodeType != LEAF_NODE)
    {
      const GE::AI::NodeTemplate& nt = tree.m_tree[i];
      const GE::AI::DisplayNode& dn = displayTree.m_displayNodes[i];
      const int& outputPin = dn.m_outputPinID;

      //Loop through and link all the parents with the children
      for (size_t j{ 0 }; j < nt.m_childrenNode.size(); ++j)
      {
        const int& inputPin = displayTree.m_displayNodes[nt.m_childrenNode[j]].m_intputPinID;
        links.m_linkList.push_back(NodeLinks(newLinkID, std::make_pair(outputPin, inputPin)));         // <output,input>
        ++newLinkID;
      }
    }
  }
  links.m_maxLinkID = newLinkID;
  m_treeList.push_back(newTree);

}

std::vector<std::pair<GE::AI::DisplayTree, GE::AI::NodeLinkList>> GE::AI::NodeEditor::GetTreeList()
{
  return m_treeList;
}
/*
void GE::AI::NodeEditor::AddRunningNode(unsigned int id)
{

  std::vector<unsigned int>::iterator it = std::find(m_currentRunningNode.begin(), m_currentRunningNode.end(), id);
  if (it == m_currentRunningNode.end())
  {
    m_currentRunningNode.push_back(id);
  }

}
void GE::AI::NodeEditor::RemoveRunningNode(unsigned int id)
{
  std::vector<unsigned int>::iterator it = std::find(m_currentRunningNode.begin(), m_currentRunningNode.end(), id);
  if (it != m_currentRunningNode.end())
  {
    m_currentRunningNode.erase(it);
  }
}
void GE::AI::NodeEditor::ClearRunningNode()
{
  m_currentRunningNode.clear();
}
*/

#ifdef _DEBUG
void GE::AI::NodeEditor::PrintDetails()
{
  for (size_t i{ 0 }; i < m_currentTree->m_displayNodes.size(); ++i)
  {
    std::cout << m_currentTree->m_displayNodes[i].m_scriptName << "\n";
    std::cout << "NodeID: " << m_currentTree->m_displayNodes[i].m_NodeID << "\n";
    std::cout << "LeftPINID: " << m_currentTree->m_displayNodes[i].m_intputPinID << "\n";
    std::cout << "RightPINID: " << m_currentTree->m_displayNodes[i].m_outputPinID << "\n\n";
  }

  for (size_t i{ 0 }; i < m_currentLinkList->m_linkList.size(); ++i)
  {
    std::cout << m_currentLinkList->m_linkList[i].m_pinIDs.first << "::" << m_currentLinkList->m_linkList[i].m_pinIDs.second << "\n";
  }
}
#endif // _DEBUG

#endif