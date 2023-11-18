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
#include <AssetManager/AssetManager.h>

namespace GE
{
	namespace Component
	{

		using namespace GE::MONO;

		struct ScriptHandler
		{

			std::map<std::string, Script> m_scriptMap;

			/*!*********************************************************************
			\brief
				Default constructor
			************************************************************************/
			ScriptHandler() {};

			/*!*********************************************************************
			\brief
				Non-default Constructor for Scripthandler. Initialize all the scripts instance and add it to the map

			\params  scriptNames
				list of the names of scripts

			\params entityID
			 ID of the entity this component belongs to
			************************************************************************/
			ScriptHandler(std::vector<std::string> const& scriptNames, unsigned int entityID)
			{

				GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
				for (const std::string& s : scriptNames)
				{
					if (m_scriptMap.find(s) == m_scriptMap.end())
					{
						try
						{
							unsigned int copy = entityID;
							std::vector<void*> arg{ &copy };
							std::vector< GE::MONO::MethodInfo> allMethodInfo{ { "Update",0 }};
							m_scriptMap[s] = Script(scriptMan->InstantiateClass(GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Player Script Namespace").c_str(), s.c_str(), arg), allMethodInfo);
						}
						catch (GE::Debug::IExceptionBase& e)
						{
							e.LogSource();
							e.Log();
							throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + s, ERRLG_FUNC, ERRLG_LINE);
						}
					}

				}
			}


			/*!*********************************************************************
			\brief
				Function to add a script to the map

			\params  scriptNames
				list of the names of scripts

			\params entityID
			 ID of the entity this component belongs to
			************************************************************************/
			void AddScript(const std::string& scriptName, unsigned int entityID) {
				if (m_scriptMap.find(scriptName) == m_scriptMap.end())
				{
					GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
					try
					{
						unsigned int copy = entityID;
						std::vector<void*> arg{ &copy };
						std::vector< GE::MONO::MethodInfo> allMethodInfo{ { "Update",0 } };
						m_scriptMap[scriptName] = Script(scriptMan->InstantiateClass(GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Player Script Namespace").c_str(), scriptName.c_str(), arg),allMethodInfo);
					}
					catch (GE::Debug::IExceptionBase& e)
					{
						e.LogSource();
						e.Log();
						throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + scriptName, ERRLG_FUNC, ERRLG_LINE);
					}

				}
			}

			/*!*********************************************************************
			\brief
				function to update all the script attacehed to the entity
			************************************************************************/
			void UpdateAllScripts()
			{
				for (std::pair<std::string, Script> cs : m_scriptMap) {
					cs.second.InvokeMethod("Update", {});
				}
			}
		};
	}
}
