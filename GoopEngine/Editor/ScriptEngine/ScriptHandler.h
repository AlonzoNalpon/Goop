#pragma once
#include <pch.h>
#include <ScriptEngine/ScriptManager.h>

namespace GE
{
	namespace Component
	{

		using namespace GE::MONO;

		struct ScriptHandler : public GE::ECS::Component
		{

			std::map<std::string,Script> m_scriptMap;

			ScriptHandler( const std::initializer_list<std::pair<std::string,std::string>>& scriptNames)
			{
				GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
				for ( const std::pair<std::string, std::string>& s: scriptNames)
				{
					if (m_scriptMap.find(s.second) == m_scriptMap.end()) {
						m_scriptMap[s.second] = Script(scriptMan->InstantiateClass(s.first.c_str(), s.second.c_str()));
					}
				
				}
			}

			void AddScript( const std::pair<std::string, std::string> &scriptName) {
				if (m_scriptMap.find(scriptName.second) == m_scriptMap.end())
				{
					GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
					//m_scriptMap[scriptName.second] = Script(scriptMan->InstantiateClass(scriptName.first.c_str(), scriptName.second.c_str()));
				}
			}


			
			void UpdateAllScripts() 
			{
				for (const std::pair<std::string,Script>& cs : m_scriptMap) {
					mono_runtime_invoke(cs.second.m_updateMethod, cs.second.m_classObjInst, nullptr, nullptr);
				}
			}
		};
	}
}
