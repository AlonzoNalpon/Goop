#pragma once
/*!*********************************************************************
\file   ScriptHandler.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for storing all the scripts attached to the entity

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <ScriptEngine/ScriptManager.h>

namespace GE
{
	namespace Component
	{

		using namespace GE::MONO;

		struct ScriptHandler
		{

			std::map<std::string,Script> m_scriptMap;

			/*!*********************************************************************
			\brief
				Non-default Constructor for Scripthandler. Initialize all the scripts instance and add it to the map

			\params  scriptNames
				list of the names of scriptes and their namespace

			\params entityID
			 ID of the entity this component belongs to
			************************************************************************/
			ScriptHandler(const std::initializer_list<std::pair<std::string, std::string>>& scriptNames, unsigned int entityID)
			{
				GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
				for ( const std::pair<std::string, std::string>& s: scriptNames)
				{
					if (m_scriptMap.find(s.second) == m_scriptMap.end()) 
					{
						try
						{
							m_scriptMap[s.second] = Script(scriptMan->InstantiateClassID(s.first.c_str(), s.second.c_str(), entityID));
						}
						catch (GE::Debug::IExceptionBase& e)
						{
							e.LogSource();
							e.Log();
							throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + s.second, ERRLG_FUNC, ERRLG_LINE);
						}
					}

				}
			}


			/*!*********************************************************************
			\brief
				Function to add a script to the map 

			\params  scriptNames
				list of the names of scriptes and their namespace

			\params entityID
			 ID of the entity this component belongs to
			************************************************************************/
			void AddScript(const std::pair<std::string, std::string>& scriptName, unsigned int entityID) {
				if (m_scriptMap.find(scriptName.second) == m_scriptMap.end())
				{
					GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
					try
					{
						m_scriptMap[scriptName.second] = Script(scriptMan->InstantiateClassID(scriptName.first.c_str(), scriptName.second.c_str(), entityID));
					}
					catch (GE::Debug::IExceptionBase& e)
					{
						e.LogSource();
						e.Log();
						throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + scriptName.second, ERRLG_FUNC, ERRLG_LINE);
					}
			
				}
			}

			/*!*********************************************************************
			\brief
				function to update all the script attacehed to the entity
			************************************************************************/
			void UpdateAllScripts() 
			{
				for (const std::pair<std::string,Script>& cs : m_scriptMap) {
					mono_runtime_invoke(cs.second.m_updateMethod, cs.second.m_classObjInst, nullptr, nullptr);
				}
			}
		};
	}
}
