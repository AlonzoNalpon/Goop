#pragma once

#include <jit/jit.h>
#include <metadata/assembly.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Math/GEM.h>


namespace GE {
	namespace MONO {

		struct Script{
			static GE::Math::dVec2 m_playPosition;
			MonoObject* m_classObjInst;
			MonoMethod* m_awakeMethod;
			MonoMethod* m_startMethod;
			MonoMethod* m_updateMethod;
			MonoMethod* m_lateUpdateMethod;

	

			Script(MonoObject* objectInstance);
			Script() : m_classObjInst { nullptr }, m_awakeMethod{ nullptr }, m_startMethod{ nullptr }, m_updateMethod{ nullptr }, m_lateUpdateMethod{ nullptr } {}
			//void CallAwake();
			//void CallStart();
			//void CallUpdate();
			//void CallLateUpdate();
			static GE::Math::dVec2 GetPlayPos();
			static void SetPlayPos(GE::Math::dVec2);
		};
		
	}
}