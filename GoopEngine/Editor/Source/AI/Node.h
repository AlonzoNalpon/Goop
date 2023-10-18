#pragma once


namespace GE
{
	namespace AI
	{

		enum NODE_STATES
		{
			NODE_IDLE,
			NODE_RUNNING,
			NODE_SUCCESS,
			NODE_FAILURE
		};

		enum NODE_TYPE
		{
			NODE_ROOT,
			NODE_ACTION,
			NODE_SELECTOR,
			NODE_SEQUENCE,
			NODE_FAIL
		};

		struct Node
		{
			using	NodeUpdateFP = NODE_STATES(*)(std::vector<Node>&);

			std::vector<Node> m_children;
			NodeUpdateFP m_behvaiourUpdate;
			NODE_TYPE	 m_nodeType;
			NODE_STATES m_nodeStatus;


			void Init(std::vector<Node>& children, NodeUpdateFP funcPointer, NODE_TYPE nodeType, NODE_STATES nodeStatus = NODE_IDLE);
			void Terminate(NODE_STATES);
			void AddChild(const Node&);
		};

	}
}



