#include "Script.h"


GE::MONO::Script::Script(MonoObject* objectInstance) :m_classObjInst{ objectInstance }
{

  MonoClass* instanceClass = mono_object_get_class(m_classObjInst);

  MonoMethod* 

  // Get a reference to the method in the class
  m_awakeMethod = mono_class_get_method_from_name(instanceClass, "Awake", 0);
 /* if (m_awakeMethod == nullptr) {
    std::cout << "Awake not found\n";
  }
  else {
    std::cout << "Awake  found\n";
  }*/

  m_startMethod = mono_class_get_method_from_name(instanceClass, "Start", 0);
  //if (m_startMethod == nullptr) {
  //  std::cout << "Start not found\n";
  //}
  //else {
  //  std::cout << "Start  found\n";
  //}


  m_updateMethod = mono_class_get_method_from_name(instanceClass, "Update", 0);
  //if (m_updateMethod == nullptr) {
  //  std::cout << "Update not found\n";
  //}
  //else {
  //  std::cout << "Update  found\n";
  //}


  m_lateUpdateMethod = mono_class_get_method_from_name(instanceClass, "LateUpdate", 0);
  //if (m_lateUpdateMethod == nullptr) {
  //  std::cout << "LateUpdate not found\n";
  //}
  //else {
  //  std::cout << "LateUpdate  found\n";
  //}

  
  mono_runtime_invoke(m_startMethod, m_classObjInst, nullptr, nullptr);
}

//
//void GE::MONO::Script::CallAwake()
//{
//  mono_runtime_invoke(m_awakeMethod, m_classObjInst, nullptr, nullptr);
//}
//
//void GE::MONO::Script::CallStart()
//{
//  mono_runtime_invoke(m_startMethod, m_classObjInst, nullptr, nullptr);
//}
//
//void GE::MONO::Script::CallUpdate()
//{
//  mono_runtime_invoke(m_updateMethod, m_classObjInst, nullptr, nullptr);
//}
//
//void GE::MONO::Script::CallLateUpdate()
//{
//  mono_runtime_invoke(m_lateUpdateMethod, m_classObjInst, nullptr, nullptr);
//}
