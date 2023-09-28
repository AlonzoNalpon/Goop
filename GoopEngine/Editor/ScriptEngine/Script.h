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


namespace GE {
	namespace MONO {

		using MonoObjectSP = std::shared_ptr<MonoObject>;

		struct Script{
			MonoObject* m_classObjInst;
			MonoMethod* m_awakeMethod;
			MonoMethod* m_startMethod;
			MonoMethod* m_updateMethod;
			MonoMethod* m_lateUpdateMethod;

			/*!*********************************************************************
			\brief
				Default constructor of Script Class
			************************************************************************/
			Script() : m_classObjInst{ nullptr }, m_awakeMethod{ nullptr }, m_startMethod{ nullptr }, m_updateMethod{ nullptr }, m_lateUpdateMethod{ nullptr } {}
	
			/*!*********************************************************************
			\brief
				Non default constructor of Script Class

			\params objectInstance
				Pointer to the instance of a MonoClass
			************************************************************************/
			Script( MonoObject* objectInstance);


		};
		
	}
}