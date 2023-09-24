#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H
#include <pch.h>
namespace GE::Component
{
  struct Model : public GE::ECS::Component
  {
    Graphics::gObjID mdlID; //!< identifier of the model
  };
}
#endif
