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
#include "../ECS/EntityComponentSystem.h"



namespace GE {
	namespace MONO {

		const size_t maxBufferSize{ 10000000 };

		enum ScriptFieldType
		{
			Void = 0,
			Bool,
			Char,
			Short,
			Int,
			Float,
			Double,
			Long,
			UShort,
			UInt,
			ULong,
			Vec2,
			Vec3,
			DVec2,
			DVec3,
			IntArr
		};

		struct ScriptField
		{
			ScriptFieldType m_fieldType;
			std::string m_fieldName;
			MonoClassField* m_classField;
		};

		struct ScriptClassInfo
		{
			MonoClass* m_scriptClass{ nullptr };
			std::map<std::string, ScriptField> m_ScriptFieldMap;

		};

		struct ScriptInstance{
			ScriptClassInfo m_scriptClassInfo{ nullptr };
			MonoObject* m_classInst{ nullptr };
			MonoMethod* m_onUpdateMethod{ nullptr };
			MonoMethod* m_onCreateMethod = { nullptr };
			inline static char m_fieldValBuffer[maxBufferSize];
			
			/*!*********************************************************************
			\brief
				Default constructor of Script Class
			************************************************************************/
			ScriptInstance() {}
	
			/*!*********************************************************************
			\brief
				Non default constructor of Script Class. 

			\params objectInstance
				Pointer to the instance of a MonoClass
			************************************************************************/
			ScriptInstance( const std::string& scriptName, std::vector<void*>& arg);

			ScriptInstance(const std::string& scriptName);


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

			void InvokeOnCreate();

			void InvokeOnUpdate(double dt);

			template<typename T>
			T GetFieldValue(const std::string& name)
			{
				if constexpr (sizeof(T) > maxBufferSize)
				{
					throw GE::Debug::Exception<ScriptInstance>(GE::Debug::LEVEL_ERROR, "Data type you provided is too big ", ERRLG_FUNC, ERRLG_LINE);
				}

				auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				if (it == m_scriptClassInfo.m_ScriptFieldMap.end())
				{
					throw GE::Debug::Exception<ScriptInstance>(GE::Debug::LEVEL_ERROR, "This class does not contain such data type " , ERRLG_FUNC, ERRLG_LINE);
				}
					
				std::cout << "Normal\n";
				const ScriptField& field = it->second;
				mono_field_get_value(m_classInst, field.m_classField, m_fieldValBuffer);
				return *(T*)m_fieldValBuffer;
			}


			MonoArray* GetFieldValueArr(const std::string& name);
		

			template<typename T>
			void SetFieldValue(const std::string& name, T value)
			{
				if constexpr (sizeof(T) > maxBufferSize)
				{
					GE::Debug::ErrorLogger::GetInstance().LogWarning("Data type you provided is too big", false);
				}
				auto it = m_scriptClassInfo.m_ScriptFieldMap.find(name);
				if (it == m_scriptClassInfo.m_ScriptFieldMap.end())
					GE::Debug::ErrorLogger::GetInstance().LogWarning("This class does not contain such data type", false);

				const ScriptField& field = it->second;
				void* val = &value;
				mono_field_set_value(m_classInst, field.m_classField, val);
			}



		};
		
	}
}