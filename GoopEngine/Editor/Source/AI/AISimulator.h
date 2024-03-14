#pragma once
/*!************************************************************************
\file EnemySystem.h
\author Han Qin Ding
\date  26-November-2023

\brief
This file contains function declaration for the Enemy System Class
The enemy system is in charge of running all the AI behaviours of the enemies.
Each behaviour tree can be used by multiple enemies.
Instead of created 1 copy of the tree for each enemy, we will only have 1 copy of the tree at all times.
Each enemy will have a tree cache. This cache will be used to keep track of the enemy's progress within the behaivour tree.
Each enemy will only resolve 1 leaf node per frame, their cache will help the system determin which node the enemy will traverse
to in the current frame.

**************************************************************************/
#pragma once
#include <ECS/System/System.h>
#include <Component/EnemyAI.h>
#include <ScriptEngine/ScriptInstance.h>
#include <AI/TreeManager.h>

namespace GE
{
	namespace AI 
	{

		//struct Sequence
		//{
		//	int m_turnCount;
		//};

		//class AISimulator : public Singleton<AISimulator>
		//{
		//	static std::vector<Sequence> m_allSequences;
		//	static int m_maxTurn;
		//	static int m_currSeq;
		//	static bool m_simStarted;
		//	static bool m_ranAllSeq;
		//	

		//public:
		//	static void StartSimulation();

		//	/*static void AddSequence( unsigned cardID, int playerHealth, int enemyHealth, MonoArray* playerHand, MonoArray* enemyHand);*/
		//	static void AddSequence(Sequence s);

		//	static void UpdateSequence(Sequence s);

		//	static Sequence GetCurrentSequence();

		//	static void EndSimulation();

		//	static bool HasSimStarted();

		//	static bool HasRanAllSeq();
		//};
	}
}
