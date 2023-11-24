#pragma once
/*!*********************************************************************
\file   ScriptManager.h
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	Script Manager Singleton in charge of initializing and cleaning the Mono.
	Provides function to retrieve C# class data
	Adds internal call into mono to allow C# to call functions defined in cpp


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <Singleton/Singleton.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ScriptEngine/ScriptInstance.h>
#include <memory>
#include "Math/GEM.h"
#include "../ECS/EntityComponentSystem.h"
#include <InputManager/InputManager.h>
#include "../Component/Transform.h"
#include <DebugTools/Exception/Exception.h>
#include <FrameRateController/FrameRateController.h>
#include <Systems/Enemy/EnemySystem.h>


namespace GE {
	namespace MONO {

	

		class ScriptManager : public Singleton<ScriptManager> {

			MonoAssembly* m_coreAssembly{ nullptr };
			std::map<std::string, ScriptClassInfo> m_monoClassMap;
		


		public:
			static std::unordered_map<std::string, ScriptFieldType> m_ScriptFieldTypeMap;
			std::vector<std::string> m_allScriptNames;
			MonoDomain* m_rootDomain{ nullptr };
			MonoDomain* m_appDomain{ nullptr };

			/*!*********************************************************************
			\brief
				Init function for Mono. Sets the assembly path, initializes the domains and Load the C# Assembly.
				Internal calls are added into mono
			************************************************************************/
			void InitMono();

			/*!*********************************************************************
			\brief
				destructor of Script Class. Calls mono's cleanup function to free the memorys and shutdown mono

			************************************************************************/
			~ScriptManager();

			MonoObject* InstantiateClass(const char* className, std::vector<void*>& arg);
			MonoObject* InstantiateClass(const char* className);

			void LoadAllMonoClass(std::ifstream& ifs);
			MonoClass* GetScriptClass(std::string className);
			ScriptField GetScriptField(std::string className, std::string fieldName);
			ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

		};


		

		/*!*********************************************************************
		\brief
			Load the C# Assembly Data from the DLL file

		\params assemblyPath
			path to the C# DLL file
		************************************************************************/
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);

		/*!*********************************************************************
		\brief
			This function returns the data of c# class. 
			The class data will be used to generate instances in other parts of the script manager

		\params assembly
		 pointer to the C# Assembly data

		\params namespaceName
		Namespace that the c# belongs in

		\params className
		Name of the c# class
		************************************************************************/
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);

		/*!*********************************************************************
		\brief
			Function to read data from a file and store it in a char buffer

		\params filepath
			path of the file

		\params outSize
			size of the file

		\return
		 char buffer containing the data of the file
		************************************************************************/
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);



		/*!*********************************************************************
		\brief
			function to Set the value of an entity's trnsform component. This function will be added as internal call
			to allow c# script to set entities' transform

		\params entity
			ID of the entity


		\params transformAdjustment
			values to be added to the entity's transform
		************************************************************************/
		static void SetPosition(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);
		static void SetScale(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);
		static void SetRotation(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);

		static GE::Math::dVec3 GetPosition(GE::ECS::Entity entity);
		static GE::Math::dVec3 GetScale(GE::ECS::Entity entity);
		static GE::Math::dVec3 GetRotation(GE::ECS::Entity entity);

		/*!*********************************************************************
		\brief
			Recursive function to calculate the greatest common denominator. This function is used
			to get the smallest size of an image

		\params large
			the larger value

		\params small
			the smaller value
		************************************************************************/
		int CalculateGCD(int large, int small);

		/*!******************************************************************
		\brief
		  Plays an animation given a name from the animation manager.

		\param animName
			Name of animation to play

		\param entity
			Entity to play the animation on
		********************************************************************/
		void PlayAnimation(std::string animName, GE::ECS::Entity entity);

		/*!******************************************************************
		\brief
			Plays an animation given a name from the animation manager.

		\param soundIterator
			An iterator to which sound in the container will be played.
			To facilliate random sound bites playing, user will 
			randomise these values between a range which the sound 
			is within the vector.

			i.e. sounds { hurt1, hurt2, groan1, groan2, groan3 }
			To play a groan sound, random between 2 & 4, and use that
			resultant value is the soundIterator

		\param entity
			Entity who holds the sound
		********************************************************************/
		void PlaySound(int soundIterator, GE::ECS::Entity entity);

		/*!*********************************************************************
		\brief 
		  Informs everyone that the current turn has finished resolving
		************************************************************************/
		void GameSystemResolved();


		int GetCardIDFromQueue(GE::ECS::Entity queueEntity, int queueIndex);

		void SetCardIDFromQueue(GE::ECS::Entity queueEntity, int queueIndex, int cardID);

		void SetPlayerHand(GE::ECS::Entity playerHand, int cardID1, int cardID2, int cardID3, int cardID4, int cardID5);

		//This Function takes in a string from c#.
		void SendString(MonoString* str);

		std::string MonoStringToSTD(MonoString* str);
		bool CheckMonoError(MonoError& error);
	}
}
