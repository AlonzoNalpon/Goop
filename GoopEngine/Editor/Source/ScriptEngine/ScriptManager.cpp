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

#include <Component/Card.h>
#include <Component/CardHolder.h>
#include <GameDef.h>
#include <Graphics/GraphicsEngine.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>
#include <Systems/Audio/AudioSystem.h>
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
      { "GoopScripts.Cards.CardBase.CardID[]", ScriptFieldType::UIntArr }

      
      //{ "System.Single[]", ScriptFieldType::FloatArr },
      //{ "System.Double[]", ScriptFieldType::DoubleArr }

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
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyTriggered", GE::Input::InputManager::GetInstance().IsKeyTriggered);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyReleased", GE::Input::InputManager::GetInstance().IsKeyReleased);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyPressed", GE::Input::InputManager::GetInstance().IsKeyPressed);
  mono_add_internal_call("GoopScripts.Mono.Utils::IsKeyHeld", GE::Input::InputManager::GetInstance().IsKeyHeld);


  // Get Functions
  mono_add_internal_call("GoopScripts.Mono.Utils::GetPosition", GE::MONO::GetPosition);
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

  // Game system stuff
  mono_add_internal_call("GoopScripts.Mono.Utils::PlayAnimation", GE::MONO::PlayAnimation);
  mono_add_internal_call("GoopScripts.Mono.Utils::GameSystemResolved", GE::MONO::GameSystemResolved);
  mono_add_internal_call("GoopScripts.Mono.Utils::PlaySound", GE::MONO::PlaySound);
  mono_add_internal_call("GoopScripts.Mono.Utils::SendString", GE::MONO::SendString);


  mono_add_internal_call("GoopScripts.Mono.Utils::SetQueueCardID", GE::MONO::SetQueueCardID);
  mono_add_internal_call("GoopScripts.Mono.Utils::SetHandCardID", GE::MONO::SetHandCardID);

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
  std::ifstream scriptNames(assetManager.GetConfigData<std::string>("ScriptNames"));
  if (scriptNames.good())
  {
     LoadAllMonoClass(scriptNames);
  }
  else
  {
    std::ifstream scriptNamesExe(assetManager.GetConfigData<std::string>("ScriptNamesExe"));
     LoadAllMonoClass(scriptNamesExe);
    
  }

}

void GE::MONO::ScriptManager::LoadAllMonoClass(std::ifstream& ifs)
{;
  std::string line{};
  while (std::getline(ifs, line)) {
    size_t commaPosition = line.find(',');
    if (commaPosition != std::string::npos) 
    {
      MonoClass* newClass = GetClassInAssembly(m_coreAssembly, line.substr(0, commaPosition).c_str(), line.substr(commaPosition + 1).c_str());
      if (newClass) 
      {
        ScriptClassInfo newScriptClassInfo{};
        newScriptClassInfo.m_scriptClass = newClass;
        //int fieldCount = mono_class_num_fields(newClass);
        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(newClass, &iterator))
        {
          const char* fieldName = mono_field_get_name(field);
          uint32_t flags = mono_field_get_flags(field);
          if (flags & FIELD_ATTRIBUTE_PUBLIC)
          {
            MonoType* type = mono_field_get_type(field);
            ScriptFieldType fieldType = MonoTypeToScriptFieldType(type);
            newScriptClassInfo.m_ScriptFieldMap[fieldName] = { fieldType, fieldName, field };
          }
        }
        m_monoClassMap[line.substr(commaPosition + 1).c_str()] = newScriptClassInfo;
        MonoMethod* ctor = mono_class_get_method_from_name(newClass, ".ctor", 0);
        if (ctor)
        {
          m_allScriptNames.push_back(line.substr(commaPosition + 1).c_str());
        }

      }
    }
  }
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
  cf.m_crossFadeStartTime[0] = fadeStart1;
  cf.m_crossFadeStartTime[1] = fadeStart2;
  cf.m_crossFadeTime = fadeDuration;

  auto as = GE::ECS::EntityComponentSystem::GetInstance().GetSystem<GE::Systems::AudioSystem>();
  as->CrossFadeAudio(cf);
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

rttr::variant  GE::MONO::ScriptManager::GetScriptFieldInst(std::string const& listType) {
  if (listType == "System.Int32")
  {
    return GE::MONO::ScriptFieldInstance<int>();
  }
  if (listType == "System.Single")
  {
    return GE::MONO::ScriptFieldInstance<float>();
  }
  if (listType == "System.Double")
  {
    return GE::MONO::ScriptFieldInstance<double>();
  }
  if (listType == "System.UInt32")
  {
    return GE::MONO::ScriptFieldInstance<unsigned>();
  }
  if (listType == "System.Int32[]")
  {
    return GE::MONO::ScriptFieldInstance < std::vector<int> > ();
  }
  if (listType == "Stats::GoopScripts.Cards.CardBase.CardID[]")
  {
    return GE::MONO::ScriptFieldInstance<std::vector<unsigned>>();

  }

  return GE::MONO::ScriptFieldInstance<std::vector<unsigned>>();
  
}

// /*!*********************************************************************
//
//    Internal Call Functions for C#
//
//************************************************************************/

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

void GE::MONO::PlayAnimation(MonoString* animName, GE::ECS::Entity entity)
{
  std::string str = GE::MONO::MonoStringToSTD(animName);
  // call play animation here
  // Error check for invalid pls!!!!!
  Graphics::gObjID spriteID{ Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(str) };
  // Doesnt set the sprite!!!
  GE::Systems::SpriteAnimSystem::SetAnimation(0, spriteID); // play anim yes!
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

int GE::MONO::CalculateGCD(int large, int small)
{
  return small == 0 ? large : CalculateGCD(small, large % small);
}

void GE::MONO::GameSystemResolved()
{
  GE::Events::EventManager::GetInstance().Dispatch(GE::Events::GameTurnResolved());
}

void  GE::MONO::SetQueueCardID(GE::ECS::Entity queueEntity, int queueIndex, int cardID)
{
  ECS::EntityComponentSystem& ecs = ECS::EntityComponentSystem::GetInstance();
  Component::CardHolder* cardHolder = ecs.GetComponent<Component::CardHolder>(queueEntity);
  ECS::Entity elemEntity = cardHolder->elements[queueIndex].elemEntity;

  if (cardHolder->elements[queueIndex].used)
  {
    cardHolder->elements[queueIndex].used = false; // no longer using this slot if it's overriden
    // But now we reenable the card that it disabled ...
    //ecs.SetIsActiveEntity(cardHolder->elements[queueIndex].cardEntity, true);
  }
  
  // Now set the sprite ...
  auto* spriteComp = ecs.GetComponent<Component::Sprite>(elemEntity);
  if (spriteComp)
  {
    auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
    spriteComp->m_spriteData.texture = texManager.GetTextureID(CardSpriteNames[cardID]);
  }
}

void  GE::MONO::SetHandCardID(GE::ECS::Entity handEntity, int handIndex, int cardID)
{
  ECS::EntityComponentSystem& ecs = ECS::EntityComponentSystem::GetInstance();
  Component::CardHolder* cardHolder = ecs.GetComponent<Component::CardHolder>(handEntity);
  ECS::Entity cardEntity = cardHolder->elements[handIndex].cardEntity;
  auto* cardComp = ecs.GetComponent<Component::Card>(cardEntity);

  // Set the card ID ...
  cardComp->cardID = static_cast<Component::Card::CardID>(cardID);

  // Now set the sprite of card ...
  auto* spriteComp = ecs.GetComponent<Component::Sprite>(cardEntity);
  if (spriteComp)
  {
    auto const& texManager = Graphics::GraphicsEngine::GetInstance().textureManager;
    spriteComp->m_spriteData.texture = texManager.GetTextureID(CardSpriteNames[cardComp->cardID]);
    ecs.SetIsActiveEntity(cardEntity, true);
  }
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
