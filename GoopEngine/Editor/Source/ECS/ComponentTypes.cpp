#include <pch.h>
#include "ComponentTypes.h"
#include <ECS/EntityComponentSystem.h>

namespace GE
{
  namespace ECS
  {
    std::vector<std::string> ECS::ComponentSignatureToString(ComponentSignature sig)
    {
      std::vector<std::string> ret{};
      unsigned index{};
      while (sig != 0)
      {
        // insert into vector if component is registered
        if (sig[1])
        {
          ret.emplace_back(componentTypes[index].get_name().to_string());
        }
        ++index;
      }

      return ret;
    }  
  } // namespace ECS
} // namespace GE