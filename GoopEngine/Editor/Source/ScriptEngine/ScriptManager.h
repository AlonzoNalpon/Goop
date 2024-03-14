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
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include "FileWatch.h"

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
#include <Fmod/FmodSystem.h>


namespace GE::MONO
{
	class ScriptManager : public Singleton<ScriptManager> 
	{
		static MonoAssembly* m_coreAssembly;
		static std::map<std::string, ScriptClassInfo> m_monoClassMap;

	public:
		static std::unordered_map<std::string, ScriptFieldType> m_ScriptFieldTypeMap;
		static std::vector<std::string> m_allScriptNames;
		static MonoDomain* m_rootDomain;
		static MonoDomain* m_appDomain;
		static std::string m_appDomFilePath;
		static std::string m_coreAssFilePath;
		static std::unique_ptr<filewatch::FileWatch<std::string>> m_fileWatcher;
		static bool m_assemblyReloadPending;
		static std::unique_ptr<filewatch::FileWatch<std::string>> m_csProjWatcher;
		static bool m_CSReloadPending;
		static bool m_rebuildCS;
		static std::string m_scnfilePath;

		/*!*********************************************************************
		\brief
			Init function for Mono. Sets the assembly path, initializes the domains and Load the C# Assembly.
			Internal calls are added into mono
		************************************************************************/
		void InitMono();

		void TestReload();
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

		static void LoadAppDomain();
		static void AddInternalCalls();
		static void LoadAssembly();
		static void ReloadAllScripts();
		static void AssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type);
		static void CSReloadEvent(const std::string& path, const filewatch::Event change_type);


		/*!*********************************************************************
		\brief
			This function is called during ScriptManager Init to create all the MonoClass*
			By having a list of all the pre-made MonoClass, user only needs to pass in the name of the
			script when they want to create an instance of a script

		\params std::ifstream& ifs
			Ifs stream to a text file containing all the script names and their namespaces
		************************************************************************/
		static void LoadAllMonoClass();

		static void ReloadAssembly(); 
		static void ReloadScripts();
		static void RebuildCS();

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
		static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

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


		MonoAssembly* GetMonoAssembly();
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

	MonoObject* GetScriptFromID(GE::ECS::Entity entity, MonoString* scriptName);
	MonoObject* GetScriptInstance(GE::ECS::Entity entityID, MonoString* scriptName);

	void SetScript(GE::ECS::Entity entity, MonoString* scriptName);

	/*!*********************************************************************
	\brief
		Sets the text component of an entity
	\param entity
		The entity to set the text component of
	\param str
		The string to set the text to
	************************************************************************/
	void SetTextComponent(GE::ECS::Entity entity, MonoString* str);

	/*!*********************************************************************
	 \brief
	   Set the text component's color of an entity.
	 \param entity
	 The entity to set the text component of
	 \param r
		red
	 \param g
		green
	 \param b
		blue
	 \param a
		alpha
	************************************************************************/
	void SetTextColor(GE::ECS::Entity entity, int r, int g, int b, int a);

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
		function to Set the value of an entity's transform component. This 
		function will be added as internal call to allow c# script to set 
		entities' transform.
	\params entity
		ID of the entity

	\params transformAdjustment
		values to be added to the entity's transform
	************************************************************************/
	static void SetPosition(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);
	static void SetScale(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);
	static void SetRotation(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);

	static GE::Math::dVec3 GetPosition(GE::ECS::Entity entity);
	static GE::Math::dVec3 GetWorldPosition(GE::ECS::Entity entity);
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
	int CalculateGCD(int large, int smaller);

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
		Gets the animation time of an animation

	\param animName
		Name of animation to play

	\return
		Animation time
	********************************************************************/
	double GetAnimationTime(MonoString* animName);

	/*!******************************************************************
	\brief
		Plays a random sound on an audio component of an entity given a
		range.
	\param startRange
		The start index of the range of sounds to play
	\param endRange
		The end index of the range
	\param entity
		Entity who holds the sound
	\param volume
		The volume to play the sound at
	********************************************************************/
	void PlayRandomSound(int startRange, int endRange, GE::ECS::Entity entity, float volume = 1.f);

	/*!*********************************************************************
	\brief 
		Forces fmod to play a sound directly.

	\param soundName
		Name of sound file
	\param volume
		Normalized volume
	\param channel
		Sound channel (BGM, SFX, Voice)
	\param looped
		If sound is looped
	************************************************************************/
	void PlaySoundF(MonoString* soundName, float volume, GE::fMOD::FmodSystem::ChannelType channel, bool looped);

	/*!*********************************************************************
	\brief 
		Stops playing a sound
		
	\param soundName
		Audio to stop playing
	************************************************************************/
	void StopSound(MonoString* soundName);

	/*!*********************************************************************
	\brief 
		Stops playing an entire channel

	\param channel
		Channel to stop playing
	************************************************************************/
	void StopChannel(GE::fMOD::FmodSystem::ChannelType channel);

	/*!*********************************************************************
	\brief 
		Informs everyone that the current turn has finished resolving
	************************************************************************/
	void GameSystemResolved();

	/*!*********************************************************************
	\brief
		Sets the whole card to inactive, except for the icon. Called when 
		queuing a card from the hand.
	\param entity
		The entity id of the card
	************************************************************************/
	void SetCardToQueuedState(GE::ECS::Entity entity, Math::dVec3 target);

	/*!*********************************************************************
	\brief
		Sets the whole card to active. Called when returning from queue to
		hand.
	\param entity
		The entity id of the card icon
	************************************************************************/
	void SetCardToHandState(unsigned cardEntity);

	/*!*********************************************************************
	\brief
		Searches the children of the entity for one named "BuffIconText"
		and sets it to active/inactive.
	\param iconID
		The entity id of the icon
	\param state
		The active state to set it to
	************************************************************************/
	void SetBuffIconTextActive(unsigned iconID, bool state);

	/*!*********************************************************************
	\brief
		function that will be called by a c# script to pass a string
		from c# to c++
	\param MonoString* str
		The string we want to pass from c# to c++
	************************************************************************/
	void SendString(MonoString* str);

	/*!*********************************************************************
	\brief
		Function to convert a C# MonoString to C++ std::string
	\param MonoString* str
	C# Monostring we want to convert
	\return
	The converted std::string
	************************************************************************/
	std::string MonoStringToSTD(MonoString* str);

	/*!*********************************************************************
	\brief
		Function to convert a std::string to C# MonoString
	\param str
		String to convert
	\return
		The converted MonoString
	************************************************************************/
	MonoString* STDToMonoString(const std::string& str);


	template<typename T>
	std::vector<T> MonoArrayToSTD(MonoArray* arr);

	/*!*********************************************************************
	\brief
		Function to check if a monostring is valid
	\param MonoError& error
	MonoError belonging to a MonoString
	\return
	bool alue to indicate if the Monostring is valid
	************************************************************************/
	bool CheckMonoError(MonoError& error);

	static bool GetLoseFocus();

	static void SetLoseFocus(bool active);

	static void SetIsActiveEntity(GE::ECS::Entity entity, bool active);

	static bool GetIsActiveEntity(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
	  Creates an instance of a prefab given its name. The position can
		be speicified as an optional argument. Note: This function does not
		subscribe an entity to a prefab's updates.
	\param key
		The name of the prefab
	\param pos
		The position to create the prefab at
	\return
	  The entity id of the created instance
	************************************************************************/
	static GE::ECS::Entity SpawnPrefab(MonoString* key, GE::Math::dVec3 pos = {});

	static int GetObjectWidth(GE::ECS::Entity entity);

	static int GetObjectHeight(GE::ECS::Entity entity);

	void SetObjectWidth(GE::ECS::Entity entity, int width);

	void SetObjectHeight(GE::ECS::Entity entity, int height);

	static GE::ECS::Entity CreateObject(MonoString* name, GE::Math::dVec3 pos = {}, GE::Math::dVec3 scale = {}, GE::Math::dVec3 rotation = {}, GE::ECS::Entity parent = ECS::INVALID_ID);

	static void UpdateSprite(GE::ECS::Entity entity, MonoString* textureName);

	void SetSpriteTint(GE::ECS::Entity entity, int r, int g, int b, int a = 255);
	/*!*********************************************************************
	\brief
		Adds a audio fade in event to the AudioSystem

	\param audio
		Name of audio

	\param endVol
		Target volume of audio 
				
	\param fadeDuration		
	************************************************************************/
	void FadeInAudio(MonoString* audio, float targetVol, float fadeDuration);

	/*!*********************************************************************
	\brief
		Adds a audio fade out event to the AudioSystem

	\param audio
		Name of audio

	\param fadeDuration
	************************************************************************/
	void FadeOutAudio(MonoString* audio, float fadeDuration);

	/*!*********************************************************************
	\brief 
		Sets an entity as a child of another
		
	\param parent
		Parent entity ID

	\param child
		Entity ID of entity being assigned
	************************************************************************/
	void SetParent(GE::ECS::Entity parent, GE::ECS::Entity child);

	/*!*********************************************************************
	\brief
		Get the parent entity of a child
	\param child
		Child entity ID
	\param child
		Entity ID of the parent and ECS::INVALID_ID otherwise
	************************************************************************/
	GE::ECS::Entity GetParentEntity(GE::ECS::Entity child);
	
	/*!*********************************************************************
	\brief
		Returns the entity id given its name
	\param entityName
		Name of the entity
	\return
		Entity ID of the entity
	************************************************************************/
	GE::ECS::Entity GetEntity(MonoString* entityName);

	/*!*********************************************************************
	\brief
		Returns the entity id given its name

	\param parent
		Parent entity id
	\param entityName
		Name of the entity

	\return
		Entity ID of the entity
	************************************************************************/
	GE::ECS::Entity GetChildEntity(GE::ECS::Entity parent, MonoString* entityName);

	/*!*********************************************************************
	\brief
		Sets the name of an entity
	\param entity
		Entity ID
	\param name
		New name of the entity
	************************************************************************/
	void SetEntityName(ECS::Entity entity, MonoString* name);

	/*!*********************************************************************
	\brief
		Returns the name of an entity
	\param entity
		Entity ID
	\return
		Name of the entity
	************************************************************************/
	MonoString* GetEntityName(ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Destroys an entity
	\param entity
		The entity to destroy
	************************************************************************/
	void DestroyEntity(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Destroys an entity
	\param name
		The name of the entity to destroy
	************************************************************************/
	void DestroyEntityByName(MonoString* name);

	/*!******************************************************************
	\brief
	  Plays an animation from the tween system of an entity.
	********************************************************************/
	void PlayTransformAnimation(GE::ECS::Entity entity, MonoString* animName);

	/*!*********************************************************************
	\brief
	  Triggers a tween animation of a parent entity and all its children.
		The animation name should be the same.
	\param parent
		The parent entity
	\param animName
		The name of the animation
	************************************************************************/
	void PlayAllTweenAnimation(GE::ECS::Entity parent, MonoString* animName);

	float GetChannelVolume(int channel);

	void SetChannelVolume(int channel, float volume);

	float GetMasterVolume();

	void SetMasterVolume(float volume);

	void SetTimeScale(float scale);

	/*!******************************************************************
	\brief
		Dispatches a quit game event
	********************************************************************/
	void DispatchQuitEvent();


#include "ScriptManager.tpp"
}
