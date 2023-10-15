#include <AI/ActionNode.h>


using namespace GE::AI;
#define UNREFERENCED_PARAMETER(P) (P);

NODE_STATES  GE::AI::FailNodeUpdate(std::vector<Node>& children)
{
	UNREFERENCED_PARAMETER(children);
	std::cout << "Fail Node\n";
	return NODE_FAILURE;
}

NODE_STATES  GE::AI::ScriptUpdate(std::vector<Node>& children)
{
	UNREFERENCED_PARAMETER(children);
	std::cout << "Sucess Node\n";
	return NODE_SUCCESS;
}
