#pragma once
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

		using MonoObjectSP = std::shared_ptr<MonoObject>;

		class ScriptManager : public Singleton<ScriptManager> {
			MonoDomain* m_rootDomain{ nullptr };
			MonoDomain* m_appDomain{ nullptr };
			MonoAssembly* m_coreAssembly{ nullptr };
			MonoClass* m_monoClass{ nullptr };

		public:
			void InitMono();
			~ScriptManager();
			MonoObject* InstantiateClass(const char* namespaceName, const char* className);
			MonoObject* InstantiateClassID(const char* namespaceName, const char* className, unsigned int entityID);


		};
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		void PrintAssemblyTypes(MonoAssembly* assembly);


		static void SetTransform(GE::ECS::Entity entity, GE::Component::Transform newChange);


		int CalculateGCD(int a, int b);
	}
}
