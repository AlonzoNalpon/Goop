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
#include <ImNode/NodeEditor.h>
#include <Component/Components.h>
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <Systems/GameSystem/GameSystem.h>
#include <Prefabs/PrefabManager.h>
#include <ObjectFactory/ObjectFactory.h>


#include <Component/Card.h>
#include <Component/CardHolder.h>
#include <GameDef.h>
#include <Graphics/GraphicsEngine.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <ScriptEngine/CSharpStructs.h>
#include <Systems/Audio/AudioSystem.h>
#include <GameStateManager/GameStateManager.h>

using namespace GE::MONO;

namespace GE 
{
  namespace MONO 
  {
    std::unordered_map<std::string, ScriptFieldType> GE::MONO::ScriptManager::m_ScriptFieldTypeMap
    {
      { "System.Boolean", ScriptFieldType::Bool },
      { "System.Char", ScriptFieldType::Char },
      { "System.Int16", ScriptFieldType::Short },
      { "System.Int32", ScriptFieldType::Int },
      { "System.Single", ScriptFieldType::Float },
      { "System.Double", ScriptFieldType::Double },
      { "System.Int64", ScriptFieldType::Long },
      { "System.UInt16", ScriptFieldType::UShort },
      { "System.UInt32", ScriptFieldType::UInt },
      { "System.UInt64", ScriptFieldType::ULong },
      { "GoopScripts.Mono.Vec2<System.Single>", ScriptFieldType::Vec2 },
      { "GoopScripts.Mono.Vec3<System.Single>", ScriptFieldType::Vec3 },
      { "GoopScripts.Mono.Vec2<System.Double>", ScriptFieldType::DVec2 },
      { "GoopScripts.Mono.Vec3<System.Double>", ScriptFieldType::DVec3 },
      { "System.Int32[]", ScriptFieldType::IntArr },
      { "GoopScripts.Cards.CardBase.CardID[]", ScriptFieldType::UIntArr },
      { "System.Collections.Queue", ScriptFieldType::QueueFT },
      { "GoopScripts.Gameplay.Deck", ScriptFieldType::DeckFT },
      { "GoopScripts.Gameplay.DeckManager", ScriptFieldType::DeckManagerFT },
      { "GoopScripts.Gameplay.CharacterType", ScriptFieldType::CharacterTypeFT },
      { "GoopScripts.UI.HealthBar", ScriptFieldType::HealthBarFT }
      
	
    };
  }
}

// /*!*********************************************************************
//
//    Mono Init Codes
//
//************************************************************************/

void GE::MONO::ScriptManager::InitMono()
{
  Assets::AssetManager& assetManager{ Assets::AssetManager::GetInstance() };
  std::ifstream file(assetManager.GetConfigData<std::string>("MonoAssemblyExeTest").c_str());
  if (file.good())
  {
    mono_set_assemblies_path(assetManager.GetConfigData<std::string>("MonoAssemblyExe").c_str());
    file.close();
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

  // Input Functions
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMouseScrollY", GE::Input::InputManager::GetInstance().GetMouseScrollVert);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMouseScrollX", GE::Input::InputManager::GetInstance().GetMouseScrollHor);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMousePos", GE::Input::InputManager::GetInstance().GetMousePos);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMousePosWorld", GE::Input::InputManager::GetInstance().GetMousePosWorld);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyTriggered", GE::Input::InputManager::GetInstance().IsKeyTriggered);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyReleased", GE::Input::InputManager::GetInstance().IsKeyReleased);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyPressed", GE::Input::InputManager::GetInstance().IsKeyPressed);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyHeld", GE::Input::InputManager::GetInstance().IsKeyHeld);


  // Get Functions
  mono_add_internal_call("GoopScripts.Mono.Utils::GetPosition", GE::MONO::GetPosition);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetWorldPosition", GE::MONO::GetWorldPosition);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetRotation", GE::MONO::GetRotation);  
  mono_add_internal_call("GoopScripts.Mono.Utils::GetScale", GE::MONO::GetScale);

  // Set Functions
  mono_add_internal_call("GoopScripts.Mono.Utils::SetPosition", GE::MONO::SetPosition);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetRotation", GE::MONO::SetRotation);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetScale", GE::MONO::SetScale);
  
  // Node Editor Functions
  mono_add_internal_call("GoopScripts.Mono.Utils::GetCurrentChildIndex", GE::Systems::EnemySystem::GetCurrentChildIndex);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetNewChildIndex", GE::Systems::EnemySystem::SetNewChildIndex);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetChildResult", GE::Systems::EnemySystem::GetChildResult);
  mono_add_internal_call("GoopScripts.Mono.Utils::RunChildNode", GE::Systems::EnemySystem::RunChildNode);
  mono_add_internal_call("GoopScripts.Mono.Utils::JumpToParent", GE::Systems::EnemySystem::JumpToParent);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetPlayerID", GE::Systems::EnemySystem::GetPlayerID);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlayerExist", GE::Systems::EnemySystem::PlayerExist);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetResult", GE::Systems::EnemySystem::SetResult);
  mono_add_internal_call("GoopScripts.Mono.Utils::ResetNode", GE::Systems::EnemySystem::ResetNode);
  mono_add_internal_call("GoopScripts.Mono.Utils::StartAI", GE::Systems::EnemySystem::StartAI);
  mono_add_internal_call("GoopScripts.Mono.Utils::EndAI", GE::Systems::EnemySystem::EndAI);

  // Game system stuff
  mono_add_internal_call("GoopScripts.Mono.Utils::GetAnimationTime", GE::MONO::GetAnimationTime);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlayAnimation", GE::MONO::PlayAnimation);
  mono_add_internal_call("GoopScripts.Mono.Utils::GameSystemResolved", GE::MONO::GameSystemResolved);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetChannelVolume", GE::MONO::GetChannelVolume);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetChannelVolume", GE::MONO::SetChannelVolume);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetMasterVolume", GE::MONO::GetMasterVolume);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetMasterVolume", GE::MONO::SetMasterVolume);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlaySound", GE::MONO::PlaySound);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlaySoundF", GE::MONO::PlaySoundF);
  mono_add_internal_call("GoopScripts.Mono.Utils::StopSound", GE::MONO::StopSound);
  mono_add_internal_call("GoopScripts.Mono.Utils::StopChannel", GE::MONO::StopChannel);
  mono_add_internal_call("GoopScripts.Mono.Utils::SendString", GE::MONO::SendString);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetScript", GE::MONO::GetScript);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetScriptFromID", GE::MONO::GetScriptFromID);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetGameSysScript", GE::MONO::GetGameSysScript);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetScriptInstanceGetScriptInstance", GE::MONO::GetScriptInstance);

  mono_add_internal_call("GoopScripts.Mono.Utils::SetCardToHandState", GE::MONO::SetCardToHandState);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetCardToQueuedState", GE::MONO::SetCardToQueuedState);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetBuffIconTextActive", GE::MONO::SetBuffIconTextActive);
  // magic
  mono_add_internal_call("GoopScripts.Mono.Utils::TransitionToScene", +[](MonoString* scene) { GE::GSM::GameStateManager::GetInstance().SetNextScene(MonoStringToSTD(scene)); });
  mono_add_internal_call("GoopScripts.Mono.Utils::GetFPS", +[](MonoString* scene) { return GE::FPS::FrameRateController::GetInstance().GetFPS(); });

  // Game UI Stuff
  mono_add_internal_call("GoopScripts.Mono.Utils::GetLoseFocus", GE::MONO::GetLoseFocus);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetLoseFocus", GE::MONO::SetLoseFocus);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetIsActiveEntity", GE::MONO::SetIsActiveEntity);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetParent", GE::MONO::SetParent);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetParentEntity", GE::MONO::GetParentEntity);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetEntity", GE::MONO::GetEntity);
  mono_add_internal_call("GoopScripts.Mono.Utils::DestroyEntity", GE::MONO::DestroyEntity);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetIsActiveEntity", GE::MONO::GetIsActiveEntity);
  mono_add_internal_call("GoopScripts.Mono.Utils::SpawnPrefab", GE::MONO::SpawnPrefab);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetObjectWidth", GE::MONO::GetObjectWidth);
  mono_add_internal_call("GoopScripts.Mono.Utils::GetObjectHeight", GE::MONO::GetObjectHeight);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetObjectWidth", GE::MONO::SetObjectWidth);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetObjectHeight", GE::MONO::SetObjectHeight);
  mono_add_internal_call("GoopScripts.Mono.Utils::CreateObject", GE::MONO::CreateObject);
  mono_add_internal_call("GoopScripts.Mono.Utils::UpdateSprite", GE::MONO::UpdateSprite);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetTextComponent", GE::MONO::SetTextComponent);

  mono_add_internal_call("GoopScripts.Mono.Utils::CrossFadeAudio", GE::MONO::CrossFadeAudio);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlayTransformAnimation", GE::MONO::PlayTransformAnimation);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetTimeScale", GE::MONO::SetTimeScale);

  

  //Load the CSharpAssembly (dll file)
  std::ifstream cAss(assetManager.GetConfigData<std::string>("CAssemblyExe"));
  if (cAss.good())
  {
    m_coreAssembly = LoadCSharpAssembly(assetManager.GetConfigData<std::string>("CAssemblyExe"));
    cAss.close();
  }
  else
  {
    m_coreAssembly = LoadCSharpAssembly(assetManager.GetConfigData<std::string>("CAssembly"));
  }
  //Load All the MonoClasses
  LoadAllMonoClass();



}

void GE::MONO::ScriptManager::LoadAllMonoClass()
{

  MonoImage* image = mono_assembly_get_image(m_coreAssembly);
  const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
  int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

  for (int32_t i = 0; i < numTypes; i++)
  {
    uint32_t cols[MONO_TYPEDEF_SIZE];
    mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

    std::string classNameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
    std::string className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
    if (classNameSpace.find("GoopScripts") != std::string::npos && classNameSpace.find("Properties") == std::string::npos)
    {
      //std::cout << classNameSpace << "::" << className << "\n";
      MonoClass* newClass = GetClassInAssembly(m_coreAssembly, classNameSpace.c_str(), className.c_str());
      if (newClass)
      {
        ScriptClassInfo newScriptClassInfo{};
        newScriptClassInfo.m_scriptClass = newClass;
        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(newClass, &iterator))
        {
          const char* fieldName = mono_field_get_name(field);
          uint32_t flags = mono_field_get_flags(field);
          if (flags & FIELD_ATTRIBUTE_PUBLIC)
          {
            MonoType* type = mono_field_get_type(field);
            ScriptFieldType fieldType = MonoTypeToScriptFieldType(type);
            std::string typeName = mono_type_get_name(type);
            //std::cout << fieldName << "\n";
            newScriptClassInfo.m_ScriptFieldMap[fieldName] = { fieldType, fieldName, field };
          }
        }
        m_monoClassMap[className] = newScriptClassInfo;
        MonoMethod* ctor = mono_class_get_method_from_name(newClass, ".ctor", 0);
        MonoMethod* ctor2 = mono_class_get_method_from_name(newClass, ".ctor", 1);
        if (ctor || ctor2)
        {
          m_allScriptNames.push_back(className);
        }

      }
    }
    

  }

      

}

MonoAssembly* GE::MONO::ScriptManager::GetMonoAssembly()
{
  return m_coreAssembly;
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

ScriptFieldType GE::MONO::ScriptManager::MonoTypeToScriptFieldType(MonoType* monoType)
{
  std::string typeName = mono_type_get_name(monoType);
  //std::cout << typeName << "::";
  auto it = m_ScriptFieldTypeMap.find(typeName);
  if (it == m_ScriptFieldTypeMap.end())
  {
    GE::Debug::ErrorLogger::GetInstance().LogWarning("Unable to access c# field type" + typeName, false);
    return ScriptFieldType::Void;
  }

  return it->second;
}
// /*!*********************************************************************
//
//    Mono Shutdown Codes
//
//************************************************************************/

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



// /*!*********************************************************************
//
//    Functions for getting C# script Data
//
//************************************************************************/


bool GE::MONO::CheckMonoError(MonoError& error)
{
  bool hasError = !mono_error_ok(&error);
  if (hasError)
  {
    unsigned short errorCode = mono_error_get_error_code(&error);
    const char* errorMessage = mono_error_get_message(&error);
    printf("Mono Error!\n");
    printf("\tError Code: %hu\n", errorCode);
    printf("\tError Message: %s\n", errorMessage);
    mono_error_cleanup(&error);
  }
  return hasError;
}

void GE::MONO::CrossFadeAudio(MonoString* audio1, float startVol1, float endVol1, float fadeStart1, float fadeEnd1,
                              MonoString* audio2, float startVol2, float endVol2, float fadeStart2, float fadeEnd2,
                              float fadeDuration)
{
  GE::Systems::AudioSystem::CrossFade cf;
  cf.m_audio[0] = MonoStringToSTD(audio1);
  cf.m_audio[1] = MonoStringToSTD(audio2);
  cf.m_startVol[0] = startVol1;
  cf.m_startVol[1] = startVol2;
  cf.m_endVol[0] = endVol1;
  cf.m_endVol[1] = endVol2;
  cf.m_crossFadeStartTime[0] = GE::GoopUtils::Lerp(0.f, fadeDuration, fadeStart1);
  cf.m_crossFadeStartTime[1] = GE::GoopUtils::Lerp(0.f, fadeDuration, fadeStart2);
  cf.m_crossFadeEndTime[0] = GE::GoopUtils::Lerp(0.f, fadeDuration, fadeEnd1);
  cf.m_crossFadeEndTime[1] = GE::GoopUtils::Lerp(0.f, fadeDuration, fadeEnd2);
  cf.m_crossFadeTime = fadeDuration;
  cf.m_currFadeTime = 0.f;
  cf.isOver = false;

  auto as = GE::ECS::EntityComponentSystem::GetInstance().GetSystem<GE::Systems::AudioSystem>();
  as->CrossFadeAudio(cf);
}

void GE::MONO::SetParent(GE::ECS::Entity parent, GE::ECS::Entity child)
{
  static auto& ecs = GE::ECS::EntityComponentSystem::GetInstance();
  ecs.SetParentEntity(child, parent);
  ecs.AddChildEntity(parent, child);
}

GE::ECS::Entity GE::MONO::GetParentEntity(GE::ECS::Entity child)
{
  auto& ecs = GE::ECS::EntityComponentSystem::GetInstance();
  return ecs.GetParentEntity(child);
}

GE::ECS::Entity GE::MONO::GetEntity(MonoString* entityName)
{
  static auto& ecs = GE::ECS::EntityComponentSystem::GetInstance();  
  return ecs.GetEntityFromName(MonoStringToSTD(entityName));
}

void GE::MONO::DestroyEntity(GE::ECS::Entity entity)
{
  static auto& ecs = GE::ECS::EntityComponentSystem::GetInstance();

  // Call OnDestroy function on entity
  // Ignore if entity is inactive
  if (ecs.GetIsActiveEntity(entity))
  {
    if (ecs.HasComponent<GE::Component::Scripts>(entity))
    {
      GE::Component::Scripts* scripts = ecs.GetComponent<GE::Component::Scripts>(entity);
      for (auto script : scripts->m_scriptList)
      {
        MonoMethod* onDestroy = mono_class_get_method_from_name(script.m_scriptClass, "OnDestroy", 1);
        if (onDestroy)
        {
          std::vector<void*> params = { &entity };
          mono_runtime_invoke(onDestroy, mono_gchandle_get_target(script.m_gcHandle), params.data(), nullptr);
        }
        mono_gchandle_free(script.m_gcHandle);
      }
    }
  }

  ecs.DestroyEntity(entity);
}

void GE::MONO::PlayTransformAnimation(GE::ECS::Entity entity, MonoString* animName)
{
  static auto& ecs = GE::ECS::EntityComponentSystem::GetInstance();
  auto tween = ecs.GetComponent<GE::Component::Tween>(entity);
  if (tween)
  {
    tween->m_playing = MonoStringToSTD(animName);
  }
}

float GE::MONO::GetChannelVolume(int channel)
{
  switch (channel)
  {
  case 0:
    return GE::fMOD::FmodSystem::GetInstance().GetChannelVolume(GE::fMOD::FmodSystem::ChannelType::BGM);
  case 1:
    return GE::fMOD::FmodSystem::GetInstance().GetChannelVolume(GE::fMOD::FmodSystem::ChannelType::SFX);
  case 2:
    return GE::fMOD::FmodSystem::GetInstance().GetChannelVolume(GE::fMOD::FmodSystem::ChannelType::VOICE);
  default:
    return 0.0f;
  }
}

void GE::MONO::SetChannelVolume(int channel, float volume)
{
  switch (channel)
  {
  case 0:
    GE::fMOD::FmodSystem::GetInstance().SetChannelVolume(GE::fMOD::FmodSystem::ChannelType::BGM, volume);
    break;
  case 1:
    GE::fMOD::FmodSystem::GetInstance().SetChannelVolume(GE::fMOD::FmodSystem::ChannelType::SFX, volume);
    break;
  case 2:
    GE::fMOD::FmodSystem::GetInstance().SetChannelVolume(GE::fMOD::FmodSystem::ChannelType::VOICE, volume);
    break;
  default:
    break;
  }
}

float GE::MONO::GetMasterVolume()
{
  return GE::fMOD::FmodSystem::GetInstance().GetMasterVolume();
}

void GE::MONO::SetMasterVolume(float volume)
{
  GE::fMOD::FmodSystem::GetInstance().SetMasterVolume(volume);
}

MonoObject* GE::MONO::ScriptManager::InstantiateClass(const char* className)
{

  // Get a reference to the class we want to instantiate
  if (m_monoClassMap.find(className) != m_monoClassMap.end())
  {
    MonoClass* currClass = m_monoClassMap[className].m_scriptClass;
    MonoObject* classInstance = mono_object_new(m_appDomain, currClass);

    // Allocate an instance of our class
    if (classInstance == nullptr)
    {
      throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Allocate memory for class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
    }
    mono_runtime_object_init(classInstance);
    return classInstance;
  }
  
  throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to locate class in map" + std::string(className), ERRLG_FUNC, ERRLG_LINE);
}

MonoObject* GE::MONO::ScriptManager::InstantiateClass( const char* className, std::vector<void*>& arg)  
{

  if (m_monoClassMap.find(className) != m_monoClassMap.end())
  {
    MonoClass* currClass = m_monoClassMap[className].m_scriptClass;
    MonoObject* classInstance = mono_object_new(m_appDomain, currClass);  //Get a reference to the class we want to instantiate

    if (classInstance == nullptr)
    {
      throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Allocate memory for class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
    }

    //Init the class through non-default constructor
    MonoMethod* classCtor = mono_class_get_method_from_name(currClass, ".ctor", static_cast<int>(arg.size()));
    if (!classCtor)
    {
      classCtor = mono_class_get_method_from_name(currClass, ".ctor", 0);
    }
    mono_runtime_invoke(classCtor, classInstance, arg.data(), nullptr);
    

    if (classInstance == nullptr) {
      throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Create the class object with non-default constructor: " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
    }
    return classInstance;
  }

  throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to locate class in map" + std::string(className), ERRLG_FUNC, ERRLG_LINE);
}

MonoClass* GE::MONO::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
  MonoImage* image = mono_assembly_get_image(assembly);
  MonoClass* klass = mono_class_from_name(image, namespaceName, className);

  if (klass == nullptr)
  {
    GE::Debug::ErrorLogger::GetInstance().LogWarning("Unable to access c# class " + std::string(className), false);
  }
  return klass;
}

MonoClass* GE::MONO::ScriptManager::GetScriptClass(std::string className)
{
  return m_monoClassMap[className].m_scriptClass;
}


ScriptClassInfo GE::MONO::ScriptManager::GetScriptClassInfo(std::string className)
{
  return m_monoClassMap[className];
}

ScriptField GE::MONO::ScriptManager::GetScriptField(std::string className, std::string fieldName)
{
  return m_monoClassMap[className].m_ScriptFieldMap[fieldName];
}

//rttr::variant  GE::MONO::ScriptManager::GetScriptFieldInst(std::string const& listType) {
//  if (listType == "System.Int32")
//  {
//    return GE::MONO::ScriptFieldInstance<int>();
//  }
//  if (listType == "System.Single")
//  {
//    return GE::MONO::ScriptFieldInstance<float>();
//  }
//  if (listType == "System.Double")
//  {
//    return GE::MONO::ScriptFieldInstance<double>();
//  }
//  if (listType == "System.UInt32")
//  {
//    return GE::MONO::ScriptFieldInstance<unsigned>();
//  }
//  if (listType == "System.Int32[]")
//  {
//    return GE::MONO::ScriptFieldInstance < std::vector<int> > ();
//  }
//  if (listType == "Stats::GoopScripts.Cards.CardBase.CardID[]")
//  {
//    return GE::MONO::ScriptFieldInstance<std::vector<unsigned>>();
//  }
//
//  //if (listType == "System.Collections.Queue")
//  //{
//  //  return GE::MONO::ScriptFieldInstance<Queue>();
//  //}
//  if (listType == "GoopScripts.Gameplay.Deck")
//  {
//    return GE::MONO::ScriptFieldInstance<Deck>();
//  }
//  if (listType == "GoopScripts.Gameplay.DeckManager")
//  {
//    return GE::MONO::ScriptFieldInstance<DeckManager>();
//  }
//  if (listType == "GoopScripts.Gameplay.CharacterType")
//  {
//    return GE::MONO::ScriptFieldInstance<unsigned>();
//  }
//
//  return GE::MONO::ScriptFieldInstance<std::vector<unsigned>>();
//  
//}

// /*!*********************************************************************
//
//    Internal Call Functions for C#
//
//************************************************************************/

void GE::MONO::SetPosition(GE::ECS::Entity entity, GE::Math::dVec3 PosAdjustment)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);

  oldTransform->m_pos = PosAdjustment;
}

void GE::MONO::SetScale(GE::ECS::Entity entity, GE::Math::dVec3 scaleAdjustment)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);

  oldTransform->m_scale = scaleAdjustment;
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

GE::Math::dVec3 GE::MONO::GetWorldPosition(GE::ECS::Entity entity)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);
  return oldTransform->m_worldPos;
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

MonoObject* GE::MONO::GetScript(MonoString* entityName, MonoString* scriptName)
{
  GE::ECS::EntityComponentSystem& ecs{ GE::ECS::EntityComponentSystem::GetInstance() };
  auto ss = ecs.GetComponent<GE::Component::Scripts>(ecs.GetEntity(MonoStringToSTD(entityName)));
  GE::MONO::ScriptInstance* scriptinst = ss->Get(MonoStringToSTD(scriptName));

  return scriptinst->m_classInst;
}

MonoObject* GE::MONO::GetScriptInstance(GE::ECS::Entity entityID, MonoString* scriptName)
{
  GE::ECS::EntityComponentSystem& ecs{ GE::ECS::EntityComponentSystem::GetInstance() };
  auto ss = ecs.GetComponent<GE::Component::Scripts>(entityID);
  GE::MONO::ScriptInstance* scriptinst = ss->Get(MonoStringToSTD(scriptName));

  return scriptinst->m_classInst;
}

MonoObject* GE::MONO::GetScriptFromID(GE::ECS::Entity entity, MonoString* scriptName)
{
  GE::ECS::EntityComponentSystem& ecs{ GE::ECS::EntityComponentSystem::GetInstance() };
  auto ss = ecs.GetComponent<GE::Component::Scripts>(ecs.GetEntity(ecs.GetEntityName(entity)));
  GE::MONO::ScriptInstance* scriptinst = ss->Get(MonoStringToSTD(scriptName));

  return scriptinst->m_classInst;
}

MonoObject* GE::MONO::GetGameSysScript(MonoString* gameSysEntityName)
{
  GE::ECS::EntityComponentSystem& ecs{ GE::ECS::EntityComponentSystem::GetInstance() };
  auto gameComp = ecs.GetComponent<GE::Component::Game>(ecs.GetEntity(MonoStringToSTD(gameSysEntityName)));
  GE::MONO::ScriptInstance& scriptinst = gameComp->m_gameSystemScript;

  return scriptinst.m_classInst;
}

void GE::MONO::PlayAnimation(MonoString* animName, GE::ECS::Entity entity)
{
  std::string str = GE::MONO::MonoStringToSTD(animName);
  // call play animation here
  // Error check for invalid pls!!!!!
  Graphics::gObjID spriteID{ Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(str) };
  // Doesnt set the sprite!!!
  GE::Systems::SpriteAnimSystem::SetAnimation(entity, spriteID); // play anim yes!
}


double  GE::MONO::GetAnimationTime(MonoString* animName)
{
  std::string str = GE::MONO::MonoStringToSTD(animName);
  size_t animID{ Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(str) };
  GE::Graphics::SpriteAnimation test = Graphics::GraphicsEngine::GetInstance().animManager.GetAnim(animID);
  return (static_cast<double>(test.frames.size()) / static_cast<double>(test.speed));
}


void GE::MONO::PlaySound(int soundIterator, GE::ECS::Entity entity)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  auto* comp = ecs->GetComponent<GE::Component::Audio>(entity);

  if (comp)
  {
    if (soundIterator < comp->m_sounds.size())
    {
      comp->Play((*(comp->m_sounds.begin() + soundIterator)).m_sound);
      return;
    }
  }
  GE::Debug::ErrorLogger::GetInstance().LogError("Trying to play a sound that does not exist from a script");
}

void GE::MONO::PlaySoundF(MonoString* soundName, float volume, GE::fMOD::FmodSystem::ChannelType channel, bool looped)
{
  static GE::fMOD::FmodSystem& fMod = GE::fMOD::FmodSystem::GetInstance();
  std::string sound = MonoStringToSTD(soundName);
  fMod.PlaySound(sound, volume, channel, looped);
}

void GE::MONO::StopSound(MonoString* soundName)
{
  static GE::fMOD::FmodSystem& fMod = GE::fMOD::FmodSystem::GetInstance();
  std::string sound = MonoStringToSTD(soundName);
  fMod.StopSound(sound);
}

void GE::MONO::StopChannel(GE::fMOD::FmodSystem::ChannelType channel)
{
  static GE::fMOD::FmodSystem& fMod = GE::fMOD::FmodSystem::GetInstance();
  fMod.StopChannel(channel);
}

int GE::MONO::CalculateGCD(int large, int small)
{
  return small == 0 ? large : CalculateGCD(small, large % small);
}

void GE::MONO::GameSystemResolved()
{
  GE::Events::EventManager::GetInstance().Dispatch(GE::Events::GameTurnResolved());
}

void GE::MONO::SetCardToQueuedState(unsigned entity, Math::dVec3 target)
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  Component::Transform* iconTrans{ nullptr };
  // iterate through all children and set to inactive
  // and enable Collider Icon
  for (ECS::Entity const& e : ecs.GetChildEntities(entity))
  {
    if (ecs.GetEntityName(e) == "Collider Icon")
    {
      iconTrans = ecs.GetComponent<Component::Transform>(e);
      ecs.SetIsActiveEntity(e, true);
      continue;
    }

    ecs.SetIsActiveEntity(e, false);
  }

  if (!iconTrans)
  {
    Debug::ErrorLogger::GetInstance().LogError("Entity " + std::to_string(entity) + " has no \"Icon\" child");
    return;
  }

  auto* cardTrans{ ecs.GetComponent<Component::Transform>(entity) };
  // offset the whole card by the vector needed
  // to get the icon to the target
  cardTrans->m_pos.x += target.x - iconTrans->m_worldPos.x;
  cardTrans->m_pos.y += target.y - iconTrans->m_worldPos.y;
}

void GE::MONO::SetCardToHandState(unsigned cardEntity)
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  // set all children of card to active and disable Collider Icon
  for (ECS::Entity const& e : ecs.GetChildEntities(cardEntity))
  {
    if (ecs.GetEntityName(e) == "Collider Icon")
    {
      ecs.SetIsActiveEntity(e, false);
      continue;
    }

    ecs.SetIsActiveEntity(e, true);
  }
}

void GE::MONO::SetBuffIconTextActive(unsigned iconID, bool state)
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  for (ECS::Entity const& e : ecs.GetChildEntities(iconID))
  {
    if (ecs.GetEntityName(e) == "BuffIconText")
    {
      ecs.SetIsActiveEntity(e, state);
      return;
    }
  }
}

void GE::MONO::SetTextComponent(GE::ECS::Entity entity, MonoString* str, float alpha)
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  Component::Text* textComp{ ecs.GetComponent<Component::Text>(entity) };
  if (!textComp)
  {
    std::ostringstream oss;
    oss << "Unable to get text component of entity " << entity;
    Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }

  textComp->m_text = MONO::MonoStringToSTD(str);
  textComp->m_clr.a = alpha;
}

void GE::MONO::SendString(MonoString* str)
{
  std::string test = GE::MONO::MonoStringToSTD(str);
  GE::Debug::ErrorLogger::GetInstance().LogMessage(test);
  //Do what ever yo want with the string
}

std::string GE::MONO::MonoStringToSTD(MonoString* str)
{
  if (str == nullptr || mono_string_length(str) == 0)
   return"";

  MonoError error;
  char* utf8 = mono_string_to_utf8_checked(str, &error);
  if (CheckMonoError(error))
    return "";

  std::string result(utf8);
  mono_free(utf8);

  return result;

}

bool GE::MONO::GetLoseFocus()
{
  return GE::Systems::GameSystem::GetLoseFocus();
}

void GE::MONO::SetLoseFocus(bool active)
{
  GE::Systems::GameSystem::SetLoseFocus(active);
}

void GE::MONO::SetIsActiveEntity(GE::ECS::Entity entity, bool active)
{
  GE::ECS::EntityComponentSystem::GetInstance().SetIsActiveEntity(entity, active);
}

bool GE::MONO::GetIsActiveEntity(GE::ECS::Entity entity)
{
  return GE::ECS::EntityComponentSystem::GetInstance().GetIsActiveEntity(entity);
}

GE::ECS::Entity GE::MONO::SpawnPrefab(MonoString* key, GE::Math::dVec3 pos)
{
  std::string str = GE::MONO::MonoStringToSTD(key);

  return GE::Prefabs::PrefabManager::GetInstance().SpawnPrefab(str, pos, false);
}

int GE::MONO::GetObjectWidth(GE::ECS::Entity entity)
{
  return GE::ECS::EntityComponentSystem::GetInstance().GetComponent<GE::Component::Sprite>(entity)->m_spriteData.info.width;
}

int GE::MONO::GetObjectHeight(GE::ECS::Entity entity)
{
  return GE::ECS::EntityComponentSystem::GetInstance().GetComponent<GE::Component::Sprite>(entity)->m_spriteData.info.height;
}

void GE::MONO::SetObjectWidth(GE::ECS::Entity entity, int width)
{
  GE::ECS::EntityComponentSystem::GetInstance().GetComponent<GE::Component::Sprite>(entity)->m_spriteData.info.width = width;
}

void GE::MONO::SetObjectHeight(GE::ECS::Entity entity, int height)
{
  GE::ECS::EntityComponentSystem::GetInstance().GetComponent<GE::Component::Sprite>(entity)->m_spriteData.info.height = height;
}

GE::ECS::Entity GE::MONO::CreateObject(MonoString* name, GE::Math::dVec3 pos, GE::Math::dVec3 scale, GE::Math::dVec3 rotation, GE::ECS::Entity parent)
{
  std::string str = GE::MONO::MonoStringToSTD(name);

  return GE::ObjectFactory::ObjectFactory::GetInstance().CreateObject(str, pos, scale, rotation, parent);
}

void GE::MONO::UpdateSprite(GE::ECS::Entity entity, MonoString* textureName)
{
  std::string str = GE::MONO::MonoStringToSTD(textureName);

  GE::ObjectFactory::ObjectFactory::GetInstance().UpdateSprite(entity, str);
}

void GE::MONO::SetTimeScale(float scale)
{
  GE::FPS::FrameRateController::GetInstance().SetTimeScale(scale);
}

