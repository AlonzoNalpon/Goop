/*!*********************************************************************
\file   ScriptManager.h
\author han.q\@digipen.edu
\date   28-September-2023
\brief
	Script Manager Singleton in charge of initializing and cleaning the Mono.
	Provides function to retrieve C# class data
	Adds internal call into mono to allow C# to call functions defined in cpp

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
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
		static std::string m_csprojPath;
		static std::string m_batfilePath;

		/*!*********************************************************************
		\brief
			Init function for Mono. Sets the assembly path, initializes the
			domains and Load the C# Assembly. Internal calls are added into mono
		************************************************************************/
		void InitMono();

		void TestReload();
		/*!*********************************************************************
		\brief
			destructor of Script Class. Calls mono's cleanup function to free
			the memory and shutdown mono
		************************************************************************/
		~ScriptManager();

		/*!*********************************************************************
		\brief
			Function to create a C# class instance by calling its non-default
			constructor and passing in the relevant arguments

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

		/*!*********************************************************************
		\brief
			Function to add internal function calls to mono
		************************************************************************/
		static void AddInternalCalls();

		/*!*********************************************************************
		\brief
			Function to load mono c# assembly
		************************************************************************/
		static void LoadAssembly();

		/*!*********************************************************************
		\brief
			Function to reload all the c# scripts from mono
		************************************************************************/
		static void ReloadAllScripts();

		/*!*********************************************************************
		\brief
			Function to pass into the file watcher. detects for any changes in the mono dll file.
			If changes detected, it will reload the assembly and the c# scripts


		\params const std::string& path
			class Name of the c# class object we want to instantiate

		\params  const filewatch::Event change_type
			The change type of the file its watching (e.g modified, add)

		************************************************************************/
		static void AssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type);

		/*!*********************************************************************
		\brief
			Function to pass into the file watcher. detects for any changes in the c# project code.
			If changes detected, it will rebuild the c# project to generate a new dll file


		\params const std::string& path
			class Name of the c# class object we want to instantiate

		\params  const filewatch::Event change_type
			The change type of the file its watching (e.g modified, add)
		************************************************************************/
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

	/*!*********************************************************************
	\brief
		Set the scale of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\param GE::Math::dVec3 PosAdjustment
		Vector 3 of the new scale
	************************************************************************/
	static void SetScale(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);

	/*!*********************************************************************
	\brief
		Set the rotation of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\param GE::Math::dVec3 PosAdjustment
		Vector 3 of the new rotation
	************************************************************************/
	static void SetRotation(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment);

	/*!*********************************************************************
	\brief
		Get the position of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\return GE::Math::dVec3
		Returns a vector 3 of the position of the entity
	************************************************************************/
	static GE::Math::dVec3 GetPosition(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Get the world position of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\return GE::Math::dVec3
		Returns a vector 3 of the world position of the entity
	************************************************************************/
	static GE::Math::dVec3 GetWorldPosition(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Get the scale of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\return GE::Math::dVec3
		Returns a vector 3 of the scale of the entity
	************************************************************************/
	static GE::Math::dVec3 GetScale(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Get the rotation of the entity
	\param GE::ECS::Entity entity
		ID of the entity
	\return GE::Math::dVec3
		Returns a vector 3 of the rotation of the entity
	************************************************************************/
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

	/*!*********************************************************************
	\brief
		Function to convert a mono array to a vector of std::string
	\param arr
		The array to convert
	\return
		The vector of strings
	************************************************************************/
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

	/*!*********************************************************************
	\brief
		Check if the application has lost focus
	************************************************************************/
	static bool GetLoseFocus();

	/*!*********************************************************************
	\brief
		Set the value of focus.
	\param bool active
		bool to set the value
	************************************************************************/
	static void SetLoseFocus(bool active);

	/*!*********************************************************************
	\brief
		Set the entity's active value.
	\param GE::ECS::Entity entity, bool active
		Set the entity id with the bool value.
	************************************************************************/
	static void SetIsActiveEntity(GE::ECS::Entity entity, bool active);

	/*!*********************************************************************
	\brief
		Get the entity's active value.
	\param GE::ECS::Entity entity
		Entity id
	************************************************************************/
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

	/*!*********************************************************************
	\brief
		Gets the sprite component of the object and get its width.
	\param GE::ECS::Entity entity
		entity ID
	\return
		Integer of the object width.
	************************************************************************/
	static int GetObjectWidth(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Gets the sprite component of the object and get its height.
	\param GE::ECS::Entity entity
		entity ID
	\return
		Integer of the object height.
	************************************************************************/
	static int GetObjectHeight(GE::ECS::Entity entity);

	/*!*********************************************************************
	\brief
		Sets the width of the sprite component of the object.
	\param GE::ECS::Entity entity
		entity ID
	\param int width
		width
	************************************************************************/
	void SetObjectWidth(GE::ECS::Entity entity, int width);

	/*!*********************************************************************
	\brief
		Sets the height of the sprite component of the object.
	\param GE::ECS::Entity entity
		entity ID
	\param int width
		height
	************************************************************************/
	void SetObjectHeight(GE::ECS::Entity entity, int height);

	/*!*********************************************************************
	\brief
		Creates an object with the parameters
	\param MonoString* name
		Entity ID
	\param GE::Math::dVec3 pos = {}
		Vector 3 of position
	\param GE::Math::dVec3 scale = {}
		Vector 3 of scale
	\param GE::Math::dVec3 rotation = {}
		Vector 3 of rotation
	\param GE::ECS::Entity parent = ECS::INVALID_ID
		Entity ID of parent
	\return
		Entity ID of the created object
	************************************************************************/
	static GE::ECS::Entity CreateObject(MonoString* name, GE::Math::dVec3 pos = {}, GE::Math::dVec3 scale = {}, GE::Math::dVec3 rotation = {}, GE::ECS::Entity parent = ECS::INVALID_ID);

	/*!*********************************************************************
	\brief
		Creates an empty entity
	\param MonoString* name
		string of the name you want to give the entity.
	\param GE::Math::dVec3 worldPos
		Vector 3 of world position
	\param GE::Math::dVec3 worldScale
		Vector 3 of world scale
	\param GE::Math::dVec3 worldRot
		Vector 3 of world rotation
	\return
		Entity ID of the created entity
	************************************************************************/
	static ECS::Entity CreateEntity(MonoString* name, GE::Math::dVec3 worldPos, GE::Math::dVec3 worldScale, GE::Math::dVec3 worldRot, GE::ECS::Entity parent);

	/*!*********************************************************************
	\brief
		Updates the sprite component of the entity
	\param GE::ECS::Entity entity
		Entity ID
	\param MonoString* textureName
		Name of the texture in the map.
	************************************************************************/
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
		Adds a tween component to an entity
	\param
		The entity to add the component to
	\param
		Whether to loop the animations
	********************************************************************/
	void AddTweenComponent(GE::ECS::Entity entity, bool loop);

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

	/*!*********************************************************************
	\brief
		Clears a tween animation of its keyframes
	\param entity
		The entity
	\param animName
		The name of the animation
	************************************************************************/
	void ClearTweenKeyFrames(GE::ECS::Entity entity, MonoString* animName);

	/*!*********************************************************************
	\brief
		Adds a keyframe to an existing tween animation
	\param entity
		The entity to add the keyframe to
	\param animName
		The name of the animation
	\param pos
		The position of the keyframe
	\param scale
		The scale of the keyframe
	\param rot
		The rotation of the keyframe
	\param duration
		The duration of the keyframe
	\param alpha
		The alpha of the sprite multiply
	************************************************************************/
	void AddTweenKeyFrame(GE::ECS::Entity entity, MonoString* animName, GE::Math::dVec3 pos, GE::Math::dVec3 scale,
		GE::Math::dVec3 rot, double duration, float alpha, MonoString* animEvent);

	/*!*********************************************************************
	\brief
		Get the volume of the channel
	\param int channel
		Specific channel
	\return float 
		Volume
	************************************************************************/
	float GetChannelVolume(int channel);

	/*!*********************************************************************
	\brief
		Set the volume of the channel
	\param int channel
		Specific channel
	\param float volume
		Volume
	************************************************************************/
	void SetChannelVolume(int channel, float volume);

	/*!*********************************************************************
	\brief
		Get the master volume of the game
	\return float
		Master volume
	************************************************************************/
	float GetMasterVolume();

	/*!*********************************************************************
	\brief
		Set the master volume of the game
	\param float volume
		Volume
	************************************************************************/
	void SetMasterVolume(float volume);

	/*!*********************************************************************
	\brief
		Set the time scale of the game
	\param float scale
		Scale in float
	************************************************************************/
	void SetTimeScale(float scale);

	/*!******************************************************************
	\brief
		Dispatches a quit game event
	********************************************************************/
	void DispatchQuitEvent();

	/*!******************************************************************
	\brief
		Plays 1 particle effect at transform

	\param trans
		Translation
	\param scale
		Scale
	\param rot
		Rotation
	\param string
		Name of sprite
	\param drag
		Drag force on particle
	\param force
		Initial Vel of the particle
	\param grav
		Gravity acting on the particle
	********************************************************************/
	void PlaySingleParticle(GE::ECS::Entity emitterEntityID);

#include "ScriptManager.tpp"
}
