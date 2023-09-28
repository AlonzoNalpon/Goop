#include "ScriptManager.h"




using namespace GE::MONO;


void GE::MONO::ScriptManager::InitMono()
{

  mono_set_assemblies_path("../lib/mono/4.5/");
  MonoDomain* rootDomain = mono_jit_init("GoopJITRuntime");
  if (rootDomain == nullptr)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to init Mono JIT", false);
    return;
  }

  //Store the root domain pointer
  m_rootDomain = rootDomain;

  //Create an App Domain
  char str[] = "GoopAppDomain";
  m_appDomain = mono_domain_create_appdomain(str, nullptr);
  mono_domain_set(m_appDomain, true);

  mono_add_internal_call("GoopScripts.Player::IsKeyTriggered", GE::Input::InputManager::GetInstance().IsKeyTriggered);
  mono_add_internal_call("GoopScripts.Player::IsKeyHeld", GE::Input::InputManager::GetInstance().IsKeyHeld);
  mono_add_internal_call("GoopScripts.Player::IsKeyReleased", GE::Input::InputManager::GetInstance().IsKeyReleased);
  mono_add_internal_call("GoopScripts.Player::IsKeyPressed", GE::Input::InputManager::GetInstance().IsKeyPressed);
  mono_add_internal_call("GoopScripts.Player::SetTransform", GE::MONO::SetTransform);
  //mono_add_internal_call("GoopScripts.Player::SetTransform", GE::ECS::SetMonoComponent<GE::Component::Transform>);
  //Retrieve the C#Assembly (.ddl file)
  try {
    m_coreAssembly = LoadCSharpAssembly("../GoopScripts/bin/Debug/GoopScripts.dll");
  }
  catch (GE::Debug::IExceptionBase& e) {
    e.LogSource();
    e.Log();
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
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Unable to open mono image", ERRLG_FUNC, ERRLG_LINE);
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



MonoObject* GE::MONO::ScriptManager::InstantiateClassID(const char* namespaceName, const char* className, unsigned int entityID)
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
  void* args =  &entityID;
  MonoMethod* classCtor = mono_class_get_method_from_name(childClass, ".ctor",1);
  mono_runtime_invoke(classCtor, classInstance, &args, nullptr);

  if (classInstance == nullptr) {
     throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Create the class object with non-default constructor: " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }
  return classInstance;
}

MonoObject* GE::MONO::ScriptManager::InstantiateClass(const char* namespaceName, const char* className)
{
  // Get a reference to the class we want to instantiate
  MonoClass* childClass = GetClassInAssembly(m_coreAssembly, namespaceName, className);

  // Allocate an instance of our class
  MonoObject* classInstance = mono_object_new(m_appDomain, childClass);

  if (classInstance == nullptr)
  {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Allocate memory for class " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }

  // Call the parameterless (default) constructor
  mono_runtime_object_init(classInstance);

  if (classInstance == nullptr) {
    throw GE::Debug::Exception<ScriptManager>(GE::Debug::LEVEL_ERROR, "Failed to Create the class object with default constructor: " + std::string(className), ERRLG_FUNC, ERRLG_LINE);
  }

  return classInstance;
}

void  GE::MONO::PrintAssemblyTypes(MonoAssembly* assembly)
{
  MonoImage* image = mono_assembly_get_image(assembly);
  const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
  int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

  for (int32_t i = 0; i < numTypes; i++)
  {
    uint32_t cols[MONO_TYPEDEF_SIZE];
    mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
    const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
    const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

    printf("%s.%s\n", nameSpace, name);
  }
}




void GE::MONO::SetTransform(GE::ECS::Entity entity, GE::Component::Transform newChange)
{
  GE::ECS::EntityComponentSystem* ecs = &(GE::ECS::EntityComponentSystem::GetInstance());
  GE::Component::Transform* oldTransform = ecs->GetComponent<GE::Component::Transform>(entity);
  std::cout << newChange.m_pos.x << "," << newChange.m_pos.x << "\n";
  oldTransform->m_pos += newChange.m_pos;
  oldTransform->m_scale += newChange.m_scale;
  oldTransform->m_rot += newChange.m_rot;
}
