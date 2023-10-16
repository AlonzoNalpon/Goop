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
#include <ScriptEngine/Script.h>
#include <memory>
#include "Math/GEM.h"
#include "../ECS/EntityComponentSystem.h"
#include <InputManager/InputManager.h>
#include "../Component/Transform.h"
#include <DebugTools/Exception/Exception.h>
#include <FrameRateController/FrameRateController.h>


namespace GE {
	namespace MONO {



		class ScriptManager : public Singleton<ScriptManager> {
			MonoDomain* m_rootDomain{ nullptr };
			MonoDomain* m_appDomain{ nullptr };
			MonoAssembly* m_coreAssembly{ nullptr };
			MonoClass* m_monoClass{ nullptr };

		public:
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
				Create an instance of a c# class. This class object isc created using its default constructor

			\params namespaceName
				Namespace that the c# belongs in

			\params className
				Name of the c# class
			************************************************************************/
			MonoObject* InstantiateClass(const char* namespaceName, const char* className);

			/*!*********************************************************************
		  \brief
		  	Create an instance of a c# class. This function is used for classes that inherits monobehaviour class.
				The class object is created using non-default construtor
		  
		  \params namespaceName
		  	Namespace that the c# belongs in
		  
		  \params className
		  	Name of the c# class
		  
		  \params entityID
		  	ID of the entity that the script belongs to
			************************************************************************/
			MonoObject* InstantiateClassID(const char* namespaceName, const char* className, unsigned int entityID);


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
		static void SetTransform(GE::ECS::Entity entity, GE::Component::Transform transformAdjustment);

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
	}
}