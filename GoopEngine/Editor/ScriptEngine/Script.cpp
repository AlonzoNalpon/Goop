#include "Script.h"



GE::MONO::Script::Script(MonoObject* objectInstance)
{
  m_classObjInst =objectInstance;
  // Get a reference to the method in the class
  MonoClass* instanceClass = mono_object_get_class(objectInstance);

  m_awakeMethod = mono_class_get_method_from_name(instanceClass, "Awake", 0);
  m_startMethod = mono_class_get_method_from_name(instanceClass, "Start", 0);
  m_updateMethod = mono_class_get_method_from_name(instanceClass, "Update", 0);
  m_lateUpdateMethod = mono_class_get_method_from_name(instanceClass, "LateUpdate", 0);
}






