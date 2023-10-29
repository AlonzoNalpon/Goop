/*!*********************************************************************
\file   ScriptManager.cpp
\author han.q\@digipen.edu
\date   28 September 2023
\brief
  Script Manager Singleton in charge of initializing and cleaning the Mono.
  Provides function to retrieve C# class data
  Adds internal call into mono to allow C# to call functions defined in cpp


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ScriptManager.h"
#include "../AssetManager/AssetManager.h"

using namespace GE::MONO;


void GE::MONO::ScriptManager::InitMono()
{
  Assets::AssetManager& assetManager{ Assets::AssetManager::GetInstance() };
  std::ifstream file("../lib/mono/4.5/mscorlib.dll");
  if (file.good())
  {
    mono_set_assemblies_path("../lib/mono/4.5/");
  }
  else
  {
    mono_set_assemblies_path(assetManager.GetConfigData<std::string>("MonoAssembly").c_str());
  }
 
 
  MonoDomain* rootDomain = mono_jit_init(assetManager.GetConfigData<std::string>("RootDomain").c_str());
  if (rootDomain == nullptr)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to init Mono JIT", false);
    return;
  }

  //Store the root domain pointer
  m_rootDomain = rootDomain;

  //Create an App Domain
  const char* str = assetManager.GetConfigData<std::string>("AppDomain").c_str();
  m_appDomain = mono_domain_create_appdomain(const_cast<char*>(str), nullptr);
  mono_domain_set(m_appDomain, true);

  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyTriggered", GE::Input::InputManager::GetInstance().IsKeyTriggered);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyHeld", GE::Input::InputManager::GetInstance().IsKeyHeld);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyReleased", GE::Input::InputManager::GetInstance().IsKeyReleased);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyPressed", GE::Input::InputManager::GetInstance().IsKeyPressed);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMouseScrollY", GE::Input::InputManager::GetInstance().GetMouseScrollVert);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMouseScrollX", GE::Input::InputManager::GetInstance().GetMouseScrollHor);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetPosition", GE::MONO::GetPosition);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetScale", GE::MONO::GetScale);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetRotation", GE::MONO::GetRotation);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetPosition", GE::MONO::SetPosition);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetScale", GE::MONO::SetScale);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetRotation", GE::MONO::SetRotation);


  mono_add_internal_call("GoopScripts.Mono.Utils::GetChildResult", GE::Systems::EnemySystem::GetChildResult);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetCurrentChildIndex", GE::Systems::EnemySystem::GetCurrentChildIndex);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetResult", GE::Systems::EnemySystem::SetResult);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetNewChildIndex", GE::Systems::EnemySystem::SetNewChildIndex);
  mono_add_internal_call("GoopScripts.Mono.Utils::RunChildNode", GE::Systems::EnemySystem::RunChildNode);
  mono_add_internal_call("GoopScripts.Mono.Utils::JumpToParent", GE::Systems::EnemySystem::JumpToParent);
  mono_add_internal_call("GoopScripts.Mono.Utils::ResetNode", GE::Systems::EnemySystem::ResetNode);

  mono_add_internal_call("GoopScripts.Mono.Utils::GetPlayerID", GE::Systems::EnemySystem::GetPlayerID);

  //Retrieve the C#Assembly (.ddl file)
//  #ifdef _DEBUG
//  try {
//    m_coreAssembly = LoadCSharpAssembly(*assetManager.GetConfigData<const char*>("CAssembly_D"));
//  }
//  catch (GE::Debug::IExceptionBase& e) {
//    e.LogSource();
//    e.Log();
//  }
//#else
//  try {
//    m_coreAssembly = LoadCSharpAssembly(*assetManager.GetConfigData<const char*>("CAssembly_R"));
//  }
//  catch (GE::Debug::IExceptionBase& e) {
//    e.LogSource();
//    e.Log();
//  }
//#endif


  std::ifstream filed("../GoopScripts/GoopScripts.dll");
  if (file.good())
  {
    m_coreAssembly = LoadCSharpAssembly("../GoopScripts/GoopScripts.dll");
  }
  else
  {
    std::ifstream filer("../GoopScripts/GoopScripts.dll");
    if (filer.good())
    {
      m_coreAssembly = LoadCSharpAssembly("../GoopScripts/GoopScripts.dll");
    }
    else
    {
      m_coreAssembly = LoadCSharpAssembly(assetManager.GetConfigData<std::string>("CAssembly_R"));
    }    
  }  
}

GE::MONO::ScriptManager::~ScriptManager()
{
  if (m_rootDomain)
  {
    mono_domain_set(mono_get_root_domain(), false);

    mono_domain_unload(m_appDomain);
    m_appDomain = nullptr;

    mono_jit_cleanup(m_rootDomain);
    m_rootDomain = nullptr;
  }
 
}

char* GE::MONO::ReadBytes(const std::string& filepath, uint32_t* outSize)
{
  std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

  if (!stream)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Unable to read for file: " + filepath, ERRLG_FUNC, ERRLG_LINE);
  }

  std::streampos end = stream.tellg();
  stream.seekg(0, std::ios::beg);
  uint32_t size = static_cast<uint32_t>(end - stream.tellg());

  if (size == 0)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "The data file: " + filepath + " is empty", ERRLG_FUNC, ERRLG_LINE);
  }

  char* buffer = new char[size];
  stream.read((char*)buffer, size);
  stream.close();

  *outSize = size;
  return buffer;
}



MonoAssembly* GE::MONO::LoadCSharpAssembly(const std::string& assemblyPath)
{
  uint32_t fileSize = 0;
  char* fileData;
  try 
  {
    fileData = ReadBytes(assemblyPath, &fileSize);
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Read file fail", ERRLG_FUNC, ERRLG_LINE);
  }


  // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
  MonoImageOpenStatus status;
  MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

  if (status != MONO_IMAGE_OK)
  {
    const char* errorMessage = mono_image_strerror(status);

    delete[] fileData;
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, errorMessage, ERRLG_FUNC, ERRLG_LINE);
  }

  MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
  mono_image_close(image);

  //Free the memory from Read Bytes
  delete[] fileData;

  return assembly;
}



MonoClass* GE::MONO::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
  MonoImage* image = mono_assembly_get_image(assembly);
  MonoClass* klass = mono_class_from_name(image, namespaceName, className);

  if (klass == nullptr)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Unable to acess c# class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }
  return klass;
}

MonoObject* GE::MONO::ScriptManager::InstantiateClass(const char* namespaceName, const char* className, void** arg, int argSize)
{
  // Get a reference to the class we want to instantiate
  MonoClass* childClass;
  try {
    childClass = GetClassInAssembly(m_coreAssembly, namespaceName, className);
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }


  // Allocate an instance of our class
  MonoObject* classInstance = mono_object_new(m_appDomain, childClass);

  if (classInstance == nullptr)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Allocate memory for class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }

  //Init the class through non-default constructor
  MonoMethod* classCtor = mono_class_get_method_from_name(childClass, ".ctor", argSize);
  mono_runtime_invoke(classCtor, classInstance, arg, nullptr);

  if (classInstance == nullptr) {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Create the class object with non-default constructor: " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }
  return classInstance;
}

MonoObject* GE::MONO::ScriptManager::InstantiateClass(const char* namespaceName, const char* className, std::vector<void*>& arg)
{
  // Get a reference to the class we want to instantiate
  MonoClass* childClass;
  try {
    childClass = GetClassInAssembly(m_coreAssembly, namespaceName, className);
  }
  catch (GE::Debug::IExceptionBase& e)
  {
    e.LogSource();
    e.Log();
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Instantiate the class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }


  // Allocate an instance of our class
  MonoObject* classInstance = mono_object_new(m_appDomain, childClass);

  if (classInstance == nullptr)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Allocate memory for class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }

  //Init the class through non-default constructor
  MonoMethod* classCtor = mono_class_get_method_from_name(childClass, ".ctor", static_cast<int>(arg.size()));
  mono_runtime_invoke(classCtor, classInstance, arg.data(), nullptr);

  if (classInstance == nullptr) {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Create the class object with non-default constructor: " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }
  return classInstance;
}


void GE::MONO::SetPosition(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment)
{
  GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);

  oldTransform->m_pos.x += (PosAdjustment.x * fpsControl->GetDeltaTime());
  oldTransform->m_pos.y += (PosAdjustment.y * fpsControl->GetDeltaTime());
}



void GE::MONO::SetScale(GE::ECS::Entity entity, GE::Math::dVec3 scaleAdjustment)
{
  GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);

  if ((scaleAdjustment.x > 1.f) && (scaleAdjustment.y > 1.f))
  {
    oldTransform->m_scale.x += (oldTransform->m_scale.x * scaleAdjustment.x * fpsControl->GetDeltaTime());
    oldTransform->m_scale.y += (oldTransform->m_scale.y * scaleAdjustment.y * fpsControl->GetDeltaTime());
  }
  else if ((scaleAdjustment.x < 1.f) && (scaleAdjustment.y < 1.f))
  {
    scaleAdjustment.x = 1.f / scaleAdjustment.x;
    scaleAdjustment.y = 1.f / scaleAdjustment.y;
    double gcd = (oldTransform->m_scale.x >= oldTransform->m_scale.y) ? static_cast<double>(CalculateGCD(static_cast<int>(oldTransform->m_scale.x), static_cast<int>(oldTransform->m_scale.y))) : static_cast<double>(CalculateGCD(static_cast<int>(oldTransform->m_scale.y), static_cast<int>(oldTransform->m_scale.x)));
    oldTransform->m_scale.x = ((oldTransform->m_scale.x - (oldTransform->m_scale.x * scaleAdjustment.x * fpsControl->GetDeltaTime())) <= scaleAdjustment.x / gcd) ? (scaleAdjustment.x / gcd) : oldTransform->m_scale.x - (oldTransform->m_scale.x * scaleAdjustment.x * fpsControl->GetDeltaTime());
    oldTransform->m_scale.y = ((oldTransform->m_scale.y - (oldTransform->m_scale.y * scaleAdjustment.y * fpsControl->GetDeltaTime())) <= scaleAdjustment.y / gcd) ? (scaleAdjustment.y / gcd) : oldTransform->m_scale.y - (oldTransform->m_scale.y * scaleAdjustment.y * fpsControl->GetDeltaTime());
  }

}

void GE::MONO::SetRotation(GE::ECS::Entity entity, GE::Math::dVec3 rotAdjustment)
{
  GE::FPS::FrameRateController* fpsControl = &(GE::FPS::FrameRateController::GetInstance());
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);

  oldTransform->m_rot.z += (rotAdjustment.z * fpsControl->GetDeltaTime());
}

GE::Math::dVec3 GE::MONO::GetPosition(GE::ECS::Entity entity)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);
  return oldTransform->m_pos;
}

GE::Math::dVec3 GE::MONO::GetScale(GE::ECS::Entity entity)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);
  return oldTransform->m_scale;
}

GE::Math::dVec3 GE::MONO::GetRotation(GE::ECS::Entity entity)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);
  return oldTransform->m_rot;
}




int GE::MONO::CalculateGCD(int large, int small)
{
  return small == 0 ? large : CalculateGCD(small, large % small);
}
