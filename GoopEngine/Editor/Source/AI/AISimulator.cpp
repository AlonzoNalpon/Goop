#include <pch.h>
#include <AI/TreeManager.h>
#include <Serialization/Serializer.h>
#include <ScriptEngine/ScriptManager.h>
#include <Serialization/Deserializer.h>
#include "../AssetManager/AssetManager.h"
#include <AI/AISimulator.h>
#include <Systems/Enemy/EnemySystem.h>

using namespace GE;
using namespace AI;


//std::vector<Sequence>AISimulator::m_allSequences{};
//
//int AISimulator::m_currSeq{};
//bool AISimulator::m_simStarted{};
//bool AISimulator::m_ranAllSeq{};
//
//void AISimulator::StartSimulation()
//{
//	m_simStarted = true;
//	m_currSeq = 0;
//}
//
//void AISimulator::AddSequence(Sequence s)
//{
//	m_allSequences.push_back(s);
//}
//
//void AISimulator::UpdateSequence(Sequence s)
//{
//	Sequence& currSeq = m_allSequences[m_currSeq];
//	currSeq = s;
//
//	if (currSeq.m_turnCount >= m_maxTurn)
//	{
//		std::cout << "Finished a sequence\n";
//		++m_currSeq;
//		if (m_currSeq >= m_allSequences.size())
//		{
//			m_ranAllSeq = true;
//		}
//	}
//}
//
//
//void AISimulator::EndSimulation()
//{
//	m_simStarted = false;
//	m_ranAllSeq = false;
//	m_allSequences.clear();
//}
//
//bool AISimulator::HasSimStarted()
//{
//	return m_simStarted;
//}
//
//bool AISimulator::HasRanAllSeq()
//{
//	return m_ranAllSeq;
//}
//
//
//Sequence AISimulator::GetCurrentSequence()
//{
//	return(m_allSequences[m_currSeq]);
//}
//
//
//
//
//
//
//
//
