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

			/*!*********************************************************************
			\brief
				Function to create a C# class instance by calling its non-default contstructor
				and passing in the relevant arguments

			\params const char* className
				class Name of the c# class object we want to instantiate

			\params std::vector<void*>& arg
			 arguments to pass into the class's non-default constructor

			 \return
			 Instance of the c# class in the form of MonoObject*
			************************************************************************/
			MonoObject* InstantiateClass(const char* className, std::vector<void*>& arg);

			/*!*********************************************************************
			\brief
				Function to create a C# class instance by calling its default contstructor
				

			\params const char* className
				class Name of the c# class object we want to instantiate

			 \return
			 Instance of the c# class in the form of MonoObject*
			************************************************************************/
			MonoObject* InstantiateClass(const char* className);

			/*!*********************************************************************
			\brief
				This function is called during ScriptManager Init to create all the MonoClass*
				By having a list of all the pre-made MonoClass, user only needs to pass in the name of the
				script when they want to create an instance of a script

			\params std::ifstream& ifs
				Ifs stream to a text file containing all the script names and their namespaces
			************************************************************************/
			void LoadAllMonoClass(std::ifstream& ifs);

			/*!*********************************************************************
			\brief
				Function to get A MonoClass* from the ScriptManager

			\params std::string className
				name of the class

			\return
			 MonoClass* obejct that will be used to generate and instance of a c# class
			************************************************************************/
			MonoClass* GetScriptClass(std::string className);

			/*!*********************************************************************
			\brief
				Function to get the script class info of  c# class

			\params std::string className
				name of the class

			\return
			 A scriptClassInfo object that contains all the details of a c# class's public data member
			************************************************************************/
			ScriptClassInfo GetScriptClassInfo(std::string className);

			/*!*********************************************************************
			\brief
				Load the C# Assembly Data from the DLL file

			\params assemblyPath
				path to the C# DLL file
			************************************************************************/
			ScriptField GetScriptField(std::string className, std::string fieldName);

			/*!*********************************************************************
			\brief
				Function to convert a monotype to my ScriptFieldType enum

			\params MonoType* monoType
			C# mono type

			\return
			A ScriptFieldTYpe enum representing the monotype
			************************************************************************/
			ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

			/*!*********************************************************************
			\brief
				return a rttr variant of the script field instance.
				Used to generate an empty script field instance of a specific template type

			\params std::string const& listType
				the template type of the scriptfield iinstance

			\return 
			an empty script field instance of a specific template type, in the form of rttr::variant
			************************************************************************/
			/*rttr::variant GetScriptFieldInst(std::string const& listType);*/

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
		  Gets the object instance of a script given the entity and script name
		\param entityName
			The name of the entity
		\param scriptName
			The name of the script
		\return
		  The object instance
		************************************************************************/
		MonoObject* GetScript(MonoString* entityName, MonoString* scriptName);

		MonoObject* GetScriptFromID(GE::ECS::Entity entity, int id);


		/*!*********************************************************************
		\brief
		  Gets the object instance of the game system script given the
			entity's name
		\param gameSysEntityName
			Name of the GameSystem entity
		\return
		  The object instance
		************************************************************************/
		MonoObject* GetGameSysScript(MonoString* gameSysEntityName);

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
		void PlayAnimation(MonoString* animName, GE::ECS::Entity entity);

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

		void SetQueueCardID(GE::ECS::Entity queueEntity, int queueIndex, int cardID);
		void SetHandCardID(GE::ECS::Entity handEntity, int handIndex, int cardID);

		/*!*********************************************************************
		\brief
			function that will be called by a c# script to pass a string from c# to c++

		\param MonoString* str
		  The string we want to pass from c# to c++
		************************************************************************/
		void SendString(MonoString* str);

		/*!*********************************************************************
		\brief
			Function to convert a C# MonoString to C++ std::String

		\param MonoString* str
		C# Monostring we want to convert

		\return
		The covnerted std::string
		************************************************************************/
		std::string MonoStringToSTD(MonoString* str);

		/*!*********************************************************************
		\brief
			Function to check if a monostring is valid

		\param MonoError& error
		MonoError belonging to a MonoString

		\return
		bool alue to indicate if the Monostring is valid
		************************************************************************/
		bool CheckMonoError(MonoError& error);

		static void SetIsActiveEntity(GE::ECS::Entity entity, bool active);

		static GE::ECS::Entity SpawnPrefab(MonoString* key, GE::Math::dVec3 pos = {}, bool mapEntity = true);

		static int GetObjectWidth(GE::ECS::Entity entity);

		static int GetObjectHeight(GE::ECS::Entity entity);

		static GE::ECS::Entity CreateObject(MonoString* name, GE::Math::dVec3 pos = {}, GE::Math::dVec3 scale = {}, GE::Math::dVec3 rotation = {}, GE::ECS::Entity parent = ECS::INVALID_ID);

		static void UpdateSprite(GE::ECS::Entity entity, MonoString* textureName);
	}
}
