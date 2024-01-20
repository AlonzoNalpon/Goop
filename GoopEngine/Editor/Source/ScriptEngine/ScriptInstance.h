#pragma once
/*!*********************************************************************
\file   ScriptInstance.h
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	Script Class to contain the data of a c# class. 
	It will store the pointer to the MonoObject and its 4 main methods


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Math/GEM.h>
#include <rttr/type.h>
#include "../ECS/EntityComponentSystem.h"
#include <ScriptEngine/ScripUtils.h>



namespace GE {
	namespace MONO {

		const size_t maxBufferSize{ 1000 };

		struct ScriptField
		{
			ScriptFieldType m_fieldType;
			std::string m_fieldName;
			MonoClassField* m_classField;

			/*!*********************************************************************
			\brief
				Function to get the all the field data from the script manager

			\params const std::string& className
				name of the script class
			************************************************************************/
			void SetField(std::string const& className);
		};

		template<typename T>
		struct ScriptFieldInstance
		{
			ScriptFieldInstance() : m_scriptField{}, m_data{} { m_type = rttr::variant(*this).get_type().get_name().to_string(); }
			ScriptFieldInstance(ScriptField const& scriptField, T data) : m_scriptField{ scriptField }, m_data{ data }
			{
				m_type = rttr::variant(*this).get_type().get_name().to_string();
			}
			std::string m_type;
			ScriptField m_scriptField;
			T m_data;

		};


		struct ScriptClassInfo
		{
			MonoClass* m_scriptClass{ nullptr };
			std::map<std::string, ScriptField> m_ScriptFieldMap;
		};


		struct ScriptInstance
		{
			std::string m_scriptName;
			GE::ECS::Entity m_entityID;
			uint32_t m_gcHandle;
			MonoClass* m_scriptClass{ nullptr };
			MonoObject* m_classInst{ nullptr };
			MonoMethod* m_onUpdateMethod{ nullptr };
			MonoMethod* m_onCreateMethod = { nullptr };

			std::vector<rttr::variant> m_scriptFieldInstList;
			inline static char m_fieldValBuffer[maxBufferSize];
			
			/*!*********************************************************************
			\brief
				Default constructor of Script Class
			************************************************************************/
			ScriptInstance() {  }
	
			/*!*********************************************************************
			\brief
				Non default constructor of Script Class. 

			\params const std::string& scriptName
				name of the script class

			\params  std::vector<void*>& arg
			arguments to pass into the scrip class's non-default constructor
			************************************************************************/
			ScriptInstance(const std::string& scriptName, std::vector<void*>& arg);

			/*!*********************************************************************
			\brief
				Non default constructor of Script Class.

			\params const std::string& scriptName
				name of the script class
			************************************************************************/

			ScriptInstance(const std::string& scriptName, GE::ECS::Entity entityID);

			/*!*********************************************************************
			\brief
				Function to get all the c# script class public field data from ScriptManager
			************************************************************************/
			void GetFields();



			/*!*********************************************************************
			\brief
				Function to invoke the OnCreate method of the c# script class
			************************************************************************/
			void InvokeOnCreate();

			/*!*********************************************************************
			\brief
				Function to invoke the OnUpdate method of the c# script class
			************************************************************************/
			void InvokeOnUpdate(GE::ECS::Entity m_entityId, double dt);


			/*!*********************************************************************
			\brief
				Template Function to get a public field from the c# script class

			\param MonoClassField* field
			pointer to the field we are trying to get

			\return 
			the value of the c# script class's public field
			************************************************************************/
			template<typename T>
			T GetFieldValue(MonoClassField* field)
			{
				mono_field_get_value(m_classInst, field, m_fieldValBuffer);
				return *(T*)m_fieldValBuffer;
			}


			/*!*********************************************************************
			\brief
				Template Function to get a public field from the c# script class.
				this function will be called when we want get the value of a script class
				public field thats an array 

			\param MonoDomain* md
			Pointer to the mono domain we are accessing

			\param MonoClassField* field
			pointer to the field we are trying to get

			\return
			the value of the c# script class's public field
			************************************************************************/
			template<typename T>
			std::vector<T> GetFieldValueArr(MonoDomain* md, MonoClassField* field)
			{
				MonoArray* newArray{};
				
				mono_field_get_value(m_classInst, field, &newArray);

				std::vector<T> test{};
				for (int i = 0; i < mono_array_length(newArray); ++i) {
					 T element = mono_array_get(newArray, T, i);
					 test.push_back(element);
				}

				return test;
			}
		

			/*!*********************************************************************
			\brief
				Template Function to set a public field from the c# script class

			\param T value
			Value we are trying to set with

			\param MonoClassField* field
			pointer to the field we are trying to get
			************************************************************************/
			template<typename T>
			void SetFieldValue( T value, MonoClassField* field)
			{
				std::memcpy(m_fieldValBuffer, &value, sizeof(T));
				mono_field_set_value(m_classInst, field, m_fieldValBuffer);
			}


			/*!*********************************************************************
			\brief
				Template Function to set a public field from the c# script class.
				this function will be called when we want set the value of a script class
				public field thats an array

			\param std::vector<T> value
			Value we want to set

			\param MonoDomain* md
			Pointer to the mono domain we are accessing

			\param MonoClassField* field
			pointer to the field we are trying to get
			************************************************************************/
			template<typename T>
			void SetFieldValueArr( std::vector<T> value, MonoDomain* md, MonoClassField* field)
			{
				MonoArray* newArray = GetMonoArray<T>(md, value.size());
				for (int i = 0; i < mono_array_length(newArray); ++i) {
					mono_array_set(newArray, int, i, value[i]);
				}
				mono_field_set_value(m_classInst, field , newArray);
			}

			/*!*********************************************************************
			\brief
				Function to update all the scriptfieldINstance in c++ with the value inside the c# script
				THis function is called to ensure the value displayed in the inspector is updated (in case the 
				values are changed inside the c# script)
			************************************************************************/
			void GetAllUpdatedFields();
		};
		
	}
}