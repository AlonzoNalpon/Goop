/*!************************************************************************
\file TreeManager.h
\author Han Qin Ding

\brief
This file contains function declaration for the TreeManager class
The Tree Manager class is in charge of serializing and deserializing the behaviour tree.
The node editor and the Enemy system will get Tree data from Tree Manager.
**************************************************************************/
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
			bool m_treeChanged = true;  //Bool to represent if its changed
		};

		class TreeManager : public Singleton<TreeManager>
		{
			std::vector<TreeTemplate> m_treeTempList;
			std::vector<TreeID> m_deletedTress; //list of IDs that belong to trees that were deleted
			bool m_listChanged; //bool to determine if we need to update the enmy system


		public:

		 /************************************************************************/ 
		 /*!
			\brief
			Tree Manager Init Function. The tree manager will get all the tree datas from the json file
			/************************************************************************/
			void Init();

		/************************************************************************/ 
		/*!
			\brief
			Tree Manager shutdown function. The tree manager will serialize all the tree data into the json file
			/************************************************************************/
			void ShutDown();

			/************************************************************************/ 
			/*!
			\brief
			Function to retrive the list of behaviour trees. This function will be called by node editor and enemy system
			when they want to get the list of behaviour trees available 

			\return
				list of behaviour tree templates
			*/
			/************************************************************************/
			std::vector<TreeTemplate>& GetTreeList();

			/************************************************************************/ 
			/*!
			\brief
			Function to retrive the list of behaviour trees's status (has been changed or not).This function will be called by node editor and enemy system
			when they want to get the list of behaviour trees status. Based on the

			\param [useCustomAllocator] bool
				list of behaviour tree's status
			*/
			/************************************************************************/
			std::vector<TreeID>& GetDelTreeList();

			/************************************************************************/ 
			/*!
			\brief
			This function will be called if the node editor wishes to update a tree or add a tree inside the TreeManager

			\param [treeTemp] TreeTemplate& 
				reference to tree Template
			*/
			/************************************************************************/
			void UpdateTreeList(TreeTemplate& treeTemp);

			/************************************************************************/
		/*!
		\brief
		This function will be called if the node editor wishes to delete inside the TreeManager

		\param [treeTemp] TreeTemplate&
			reference to tree Template
		*/
		/************************************************************************/
			void DeleteTree(unsigned pos);

			/************************************************************************/ 
			/*!
			\brief
			This function will be called if the enemy systems wants to check if any of the tree has been changed.
			If any of the trees has been changed or a new tree is added, the enemy system will have to update the tree data on its side

			\return
				bool variable to determine whether any of the trees has been changed or if a tree is added
			*/
			/************************************************************************/
			bool isTreeUpdated();

			/************************************************************************/ 
			/*!
			\brief
			This function will be called by the enemy system when its done updating the tree data on its side.
			This is prevents the enemy system from having to constantly iterate through each tree to check if a tree is updated
			when no trees have been changed
			

			\param [cond] bool
				bool variable to determine whether the tree list in the tree manager is changed
			*/
			/************************************************************************/
			void SetTreeBool(bool cond);

		};
	}
}
