#pragma once
#include "../Singleton/Singleton.h"
#include <AI/Tree.h>
#include <Math/GEM.h>


namespace GE {
	namespace AI {

		struct NodeTemplate
		{
			NODE_TYPE m_nodeType;
			NodeID m_parentNode;
			std::vector<NodeID> m_childrenNode;
			std::string m_scriptName;
			GE::Math::Vec2 m_pos;
		};

		struct TreeTemplate
		{
			std::vector<NodeTemplate> m_tree;
			std::string m_treeName;
			TreeID m_treeTempID;

		};

		class TreeManager : public Singleton<TreeManager>
		{
			std::vector<std::pair<bool,TreeTemplate>> m_treeTempList; //Bool to represent if its changed
			bool m_listChanged;

		public:

			void Init();
			void ShutDown();
			std::vector<std::pair<bool, TreeTemplate>>& GetTreeList();
			void UpdateTreeList(TreeTemplate& treeTemp);
			bool isTreeUpdated();
			void SetTreeBool(bool cond);

		};
	}
}
