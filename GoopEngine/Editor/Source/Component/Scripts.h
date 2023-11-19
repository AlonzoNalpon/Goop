#pragma once
/*!*********************************************************************
\file   Scripts.h
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

		struct Scripts
		{
			ECS::Entity  m_entityId;
			std::map<std::string, ScriptInstance> m_scriptMap;

			Scripts() {}

			/*!*********************************************************************
			\brief
				Non-default Constructor for Scripthandler. Initialize all the scripts instance and add it to the map

			\params  scriptNames
				list of the names of scripts

			\params entityID
			 ID of the entity this component belongs to
			************************************************************************/
			Scripts(std::vector<std::string> const& scriptNames, unsigned int entityID) : m_entityId{ entityID }
			{
				for (const std::string& s : scriptNames)
				{
					if (m_scriptMap.find(s) == m_scriptMap.end())
					{
						try
						{
							unsigned int copy = entityID;
							std::vector<void*> arg{ &copy };
							m_scriptMap[s] = ScriptInstance(s,arg);
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
				UNREFERENCED_PARAMETER(scriptName);
				UNREFERENCED_PARAMETER(entityID);
				/*if (m_scriptMap.find(scriptName) == m_scriptMap.end())
				{
					GE::MONO::ScriptManager* scriptMan = &(GE::MONO::ScriptManager::GetInstance());
					try
					{
						unsigned int copy = entityID;
						std::vector<void*> arg{ &copy };
						std::vector< GE::MONO::MethodInfo> allMethodInfo{ { "Update",0 } };
						m_scriptMap[scriptName] = ScriptInstance(scriptMan->InstantiateClass(GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Player Script Namespace").c_str(), scriptName.c_str(), arg),allMethodInfo);
					}
					catch (GE::Debug::IExceptionBase& e)
					{
						e.LogSource();
						e.Log();
						throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + scriptName, ERRLG_FUNC, ERRLG_LINE);
					}

				}*/
			}

			/*!*********************************************************************
			\brief
				function to update all the script attacehed to the entity
			************************************************************************/
			void UpdateAllScripts()
			{
				double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
				for (std::pair<std::string, ScriptInstance> cs : m_scriptMap) {
					cs.second.InvokeOnUpdate(dt);
				}
			}
		};
	}
}
