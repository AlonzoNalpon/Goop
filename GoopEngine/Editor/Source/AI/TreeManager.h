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
			TreeTemplate() = default;
			TreeTemplate(std::vector<NodeTemplate> const& tree, std::string treeName, TreeID treeTempID)
				: m_tree{ tree }, m_treeName{ std::move(treeName) }, m_treeTempID{ treeTempID } {}
			TreeTemplate(std::vector<NodeTemplate>&& tree, std::string treeName, TreeID treeTempID)
				: m_tree{ tree }, m_treeName{ std::move(treeName) }, m_treeTempID{ treeTempID } {}

			std::vector<NodeTemplate> m_tree;
			std::string m_treeName;
			TreeID m_treeTempID;
		};

		class TreeManager : public Singleton<TreeManager>
		{
			std::vector<TreeTemplate> m_treeTempList; //Bool to represent if its changed
			std::vector<bool> m_treeTempCond;
			bool m_listChanged;

		public:

			void Init();
			void ShutDown();
			std::vector<TreeTemplate>& GetTreeList();
			std::vector<bool>& GetTreeCondList();
			void UpdateTreeList(TreeTemplate& treeTemp);
			bool isTreeUpdated();
			void SetTreeBool(bool cond);

		};
	}
}
