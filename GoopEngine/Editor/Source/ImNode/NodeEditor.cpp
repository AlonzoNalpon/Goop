#include <pch.h>
#include <ImNode/NodeEditor.h>
#include <algorithm>
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>



void GE::AI::NodeEditor::NodeEditorInit()
{
  // Get all the names of the scripts available
  std::filesystem::path aiDir{ "../GoopScripts/AI" };
  if (!std::filesystem::exists(aiDir))
  {
    throw Debug::Exception<GE::AI::NodeEditor>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to open GoopScripts AI folder: " + aiDir.string()));
  }
  for (const auto& file : std::filesystem::recursive_directory_iterator(aiDir))
  {
    if (!file.is_regular_file()) { continue; }	// skip if file is a directory
    std::string const& currExt{ file.path().extension().string() };
    if (currExt == scriptExt)	// image
    {
      m_allScriptNames.push_back(file.path().stem().string());
    }
  }

  //Create the node editor context and turn on any settings
  ImNodes::CreateContext();
  ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
  ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

  //Get all the trees
  const std::vector<TreeTemplate>& tempTreeList = GE::AI::TreeManager::GetInstance().GetTreeList();

  for (size_t i{ 0 }; i < tempTreeList.size(); ++i)
  {
    AddDisplayTree(tempTreeList[i]);
  }

  m_currentTree = (m_treeList.size() != 0) ? &(m_treeList[0].first) : nullptr;
  m_currentLinkList = (m_treeList.size() != 0) ? &(m_treeList[0].second) : nullptr;
  m_currentTreeInd = (m_treeList.size() != 0) ? 0 : static_cast<unsigned>(EMPTYID);

}

void  GE::AI::NodeEditor::NodeEditorShow()
{

  ImGui::Begin("Node editor");
  ImNodes::BeginNodeEditor();

  if (m_currentTree != nullptr)
  {
    m_displayPopup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1);
    ShowTree();
  }

  ImNodes::EndNodeEditor();

  if (m_currentTree != nullptr)
  {
    // Check if any new links have been made
    NodeLinks createdLink{};
    if (ImNodes::IsLinkCreated(&createdLink.m_pinIDs.first, &createdLink.m_pinIDs.second))
    {
      m_currentTree->m_changedData = true;
      createdLink.m_linkID = m_currentLinkList->m_currLinkID++;
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

  ImGui::End();

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
        dispNode.m_intputPinID = m_currentTree->m_currPinID++;
        dispNode.m_outputPinID = m_currentTree->m_currPinID++;
        dispNode.m_NodeID = m_currentTree->m_curNodeID++;
        dispNode.m_nodeType = static_cast<GE::AI::NODE_TYPE>(i);
        ImNodes::SetNodeScreenSpacePos(static_cast<int>(m_currentTree->m_displayNodes.size()), nodePos);
        dispNode.m_pos = ImNodes::GetNodeGridSpacePos(static_cast<int>(m_currentTree->m_displayNodes.size()));

        m_currentTree->m_displayNodes.push_back(dispNode);
        //m_currentTree->m_changedData = true;
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
            //PrintDetails();
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
            /*std::cout << "\nAFT\n";
            PrintDetails();*/
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
  std::cout << "-----------------------------------------------------------------------------\n";
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
      for (size_t j{ 0 }; j < childPinID.size(); ++j)
      {
        if ((m_currentTree->m_displayNodes[i].m_intputPinID == childPinID[j]))
        {

          nodeTemp.m_childrenNode.push_back(static_cast<NodeID>(i));
        }
      }
    }
    std::cout << dispNode.m_scriptName << ":" << currID << "\n";
    //std::cout << "Parent:\n";
    //std::cout << nodeTemp.m_parentNode << "\n";
    std::cout << "CHILD:\n";
    for (NodeID n : nodeTemp.m_childrenNode)
    {
      std::cout << n << "\n";
    }

    std::cout << "\n";
    ++currID;
    treeTemp.m_tree.push_back(nodeTemp);
  }
  std::cout << "-----------------------------------------------------------------------------\n";
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
    int inputPinID = displayTree.m_currPinID++; //Root node has no parent
    int outputPinID = displayTree.m_currPinID++; //Leaf node has no child
    DisplayNode newDisplayNode = DisplayNode(nt.m_scriptName, outputPinID, inputPinID, currNodeID, nt.m_nodeType, ImVec2(nt.m_pos.x, nt.m_pos.y));
    displayTree.m_displayNodes.push_back(newDisplayNode);
    ++currNodeID;
  }
  displayTree.m_treeName = tree.m_treeName;
  displayTree.m_treeID = tree.m_treeTempID;
  displayTree.m_curNodeID = currNodeID;


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
  links.m_currLinkID = newLinkID;
  m_treeList.push_back(newTree);

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

