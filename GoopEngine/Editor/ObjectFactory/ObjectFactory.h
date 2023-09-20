#pragma once
#include "../pch.h"


namespace GE
{
  namespace ObjectFactory
  {
    class ObjectFactory : public Singleton<ObjectFactory>
    {
    public:

      ECS::Entity CreateEntity();

    private:

    };
  }
}
