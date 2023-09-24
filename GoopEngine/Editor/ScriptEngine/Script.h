#pragma once

#include <jit/jit.h>
#include <metadata/assembly.h>
#include <string>
#include <iostream>
#include <fstream>


namespace GE {
	namespace MONO {

		struct Script{

			MonoObject* m_classObjInst;
			MonoMethod* m_awakeMethod;
			MonoMethod* m_startMethod;
			MonoMethod* m_updateMethod;
			MonoMethod* m_lateUpdateMethod;

	

			Script(MonoObject* objectInstance);
			//void CallAwake();
			//void CallStart();
			//void CallUpdate();
			//void CallLateUpdate();
		};


	}
}