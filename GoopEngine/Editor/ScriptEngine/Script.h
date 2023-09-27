#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Math/GEM.h>


namespace GE {
	namespace MONO {

		struct Script{
			MonoObject* m_classObjInst;
			MonoMethod* m_awakeMethod;
			MonoMethod* m_startMethod;
			MonoMethod* m_updateMethod;
			MonoMethod* m_lateUpdateMethod;

	

			Script( MonoObject* objectInstance);
			Script() : m_classObjInst { nullptr }, m_awakeMethod{ nullptr }, m_startMethod{ nullptr }, m_updateMethod{ nullptr }, m_lateUpdateMethod{ nullptr } {}
		};
		
	}
}