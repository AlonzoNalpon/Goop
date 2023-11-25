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

			void inline SetMyScriptField(const std::string& className) { m_scriptField.SetField(className); }
		};


		struct ScriptClassInfo
		{
			MonoClass* m_scriptClass{ nullptr };
			std::map<std::string, ScriptField> m_ScriptFieldMap;
		};


		struct ScriptInstance
		{
			std::string m_scriptName;
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

			\params objectInstance
				Pointer to the instance of a MonoClass
			************************************************************************/
			ScriptInstance(const std::string& scriptName, std::vector<void*>& arg);

			ScriptInstance(const std::string& scriptName);

			void GetFields();


			/*!*********************************************************************
			\brief
				Invoke a specific method of the C# script. You just need to pass in the method name and its arguments.
				Example of how to use it:

				int arg1, arg2;
				std::vector<void*> myArg {&arg1,arg2};
				InvokeMethod( methodName, myArg.data() );

			\params methodName
				Name of the method
				
			\params arg
				argument list of the method. 
				You can store all the actual arguments inside std::vector<void*>. The pass in the vector.arg() as this param

			\return 
				whatever return value that c# method returns, but as a MonoOBject pointer. To access it in c++, cast it to oriiginal data type (e.g. (int*) )
			************************************************************************/

			void Clear();

			void InvokeOnCreate();

			void InvokeOnUpdate(double dt);

			template<typename T>
			T GetFieldValue(MonoClassField* field)
			{
				/*if constexpr (sizeof(T) > maxBufferSize)
				{
					throw GE::Debug::Exception<ScriptInstance>(GE::Debug::LEVEL_ERROR, "Data type you provided is too big ", ERRLG_FUNC, ERRLG_LINE);
				}

				auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				if (it == m_scriptClassInfo.m_ScriptFieldMap.end())
				{
					throw GE::Debug::Exception<ScriptInstance>(GE::Debug::LEVEL_ERROR, "This class does not contain such data type " , ERRLG_FUNC, ERRLG_LINE);
				}*/
					
				mono_field_get_value(m_classInst, field, m_fieldValBuffer);
				return *(T*)m_fieldValBuffer;
			}


			template<typename T>
			std::vector<T> GetFieldValueArr(MonoDomain* md, MonoClassField* field)
			{
				//auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				//MonoClassField* field = it->second.m_classField;
				MonoArray* newArray{};
				
				mono_field_get_value(m_classInst, field, &newArray);

				std::vector<T> test{};
				for (int i = 0; i < mono_array_length(newArray); ++i) {
					 T element = mono_array_get(newArray, T, i);
					 test.push_back(element);
				}

				return test;
			}
		

			template<typename T>
			void SetFieldValue( T value, MonoClassField* field)
			{
				//if constexpr (sizeof(T) > maxBufferSize)
				//{
				//	GE::Debug::ErrorLogger::GetInstance().LogWarning("Data type you provided is too big", false);
				//}
				//auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				//if (it == m_scriptClassInfo.m_ScriptFieldMap.end())
				//	GE::Debug::ErrorLogger::GetInstance().LogWarning("This class does not contain such data type", false);

				//const ScriptField& field = it->second;


				std::memcpy(m_fieldValBuffer, &value, sizeof(T));
				mono_field_set_value(m_classInst, field, m_fieldValBuffer);
			}


			template<typename T>
			void SetFieldValueArr( std::vector<T> value, MonoDomain* md, MonoClassField* field)
			{
				//if constexpr (sizeof(T) > maxBufferSize)
				//{
				//	GE::Debug::ErrorLogger::GetInstance().LogWarning("Data type you provided is too big", false);
				//}
				//auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				//if (it == m_scriptClassInfo.m_ScriptFieldMap.end())
				//	GE::Debug::ErrorLogger::GetInstance().LogWarning("This class does not contain such data type", false);


				MonoArray* newArray = GetMonoArray<T>(md, value.size());
				for (int i = 0; i < mono_array_length(newArray); ++i) {
					mono_array_set(newArray, int, i, value[i]);
				}

				//const ScriptField& field = it->second;
				mono_field_set_value(m_classInst, field , newArray);
			}


			void GetAllUpdatedFields();
		};
		
	}
}