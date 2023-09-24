#pragma once
#include <Singleton/Singleton.h>
#include <jit/jit.h>
#include <metadata/assembly.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ScriptEngine/Script.h>


namespace GE {
	namespace MONO {

		class ScriptManager : public Singleton<ScriptManager > {
			MonoDomain* m_rootDomain{ nullptr };
			MonoDomain* m_appDomain{ nullptr };
			MonoAssembly* m_coreAssembly{ nullptr };

		public:
			void InitMono();
			void UpdateMono();
			MonoObject* InstantiateClass(const char* namespaceName, const char* className);

		};
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		void PrintAssemblyTypes(MonoAssembly* assembly);
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		void CallIncrementFloatVarMethod(MonoObject* objectInstance, float value);
		void CallPrintFloatVarMethod(MonoObject* objectInstance);

	}
}
