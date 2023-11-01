	#pragma once
#include <ImNode/imnodes.h>
#include <AI/TreeManager.h>
#include <vector>
#include "../Singleton/Singleton.h"


namespace GE {
	namespace AI{

		using PinID = int;
		using LinkID = int;


		struct DisplayNode
		{
			std::string m_scriptName;
			PinID  m_outputPinID;
			PinID  m_intputPinID;
			NodeID m_NodeID;
			NODE_TYPE m_nodeType;
			ImVec2 m_pos;
		};

		struct DisplayTree
		{
			std::string m_treeName;
			TreeID m_treeID;
			NodeID m_curNodeID;
			PinID  m_currPinID;
			bool m_changedData;
			bool m_changedPos;
			std::vector<DisplayNode> m_displayNodes;
		};

		struct NodeLinks
		{
			LinkID m_linkID;
			std::pair<PinID, PinID> m_pinIDs;  // <start,end>, <output, intput>
		};

		struct NodeLinkList
		{
			LinkID m_currLinkID;
			std::vector<NodeLinks> m_linkList; //LMAO link list thats not a linked list
		};
		
		struct NodeRGBA
		{
			int r,g,b,a;
		};


		class NodeEditor : public Singleton<NodeEditor>
		{
			const PinID EMPTYID = -1;
			const std::vector<std::string> nodeTitles{ "Root Node","Composite Node","Leaf Node" };
			const std::vector<std::string> addNodeOption{ "Add Root Node","Add Composite Node","Add Leaf Node" };
			const std::vector<std::string> editNodeOption{"Change to Root Node","Change to Composite Node","Change to Leaf Node", "Delete Node"};
			const std::vector<NodeRGBA> titleColors{ {110, 93, 175, 255},{212, 172, 14, 255},{153, 0, 76, 255} };
			const std::vector<NodeRGBA> nodeBgColors{ {0, 204, 102, 255},{59, 59, 59, 255} };
			const ImVec2 nodeDispSize{ 170.0f, 0.0f };
			const std::string scriptExt{ ".cs" };

			std::vector<std::pair<DisplayTree, NodeLinkList>> m_treeList;

			std::vector<std::string> m_allScriptNames;
			GE::AI::DisplayTree* m_currentTree;
			GE::AI::NodeLinkList* m_currentLinkList;
			unsigned int m_currentTreeInd;
			int m_selectedNodeInd;
			bool m_displayPopup;





		public:
			void NodeEditorInit();
			void NodeEditorShow();
			void NodeEditorShutdown();


			void ShowTree();
			void DisplayPopup();

			void UpdateNewTree();
			void AddDisplayTree(const TreeTemplate& tree);

			//void AddNode();


			//static void AddRunningNode(unsigned int id);
			//static void RemoveRunningNode(unsigned int id);
			//static void ClearRunningNode();

		};

	}
}