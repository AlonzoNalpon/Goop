#pragma once
/*!*********************************************************************
\file   Script.h
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
#include <map>


namespace GE {
	namespace MONO {

		using MethodInfo = std::pair<std::string, int>;

		struct Script{
			MonoObject* m_classObjInst;
			std::map<std::string, MonoMethod*> m_classMethods;


			/*!*********************************************************************
			\brief
				Default constructor of Script Class
			************************************************************************/
			Script() : m_classObjInst{ nullptr }, m_classMethods{} {}
	
			/*!*********************************************************************
			\brief
				Non default constructor of Script Class. 

			\params objectInstance
				Pointer to the instance of a MonoClass
			************************************************************************/
			Script(MonoObject* objectInstance, const std::vector<std::pair<std::string, int>>& allMethodInfo);


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
			MonoObject* InvokeMethod(const std::string& methodName, void** arg);


		};
		
	}
}