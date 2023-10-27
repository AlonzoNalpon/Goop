#include <pch.h>
#include "ComponentTypes.h"

namespace GE
{
  namespace ECS
  {
    std::vector<std::string> ECS::ComponentSignatureToString(ComponentSignature sig)
    {
      std::vector<std::string> ret{};
      for (auto const& elem : componentsToString)
      {
        // insert into vector if component is registered
        if (IsBitSet(sig, elem.first))
        {
          ret.emplace_back(componentsToString.at(elem.first));
        }
      }

      return ret;
    }

    ECS::COMPONENT_TYPES& operator++(ECS::COMPONENT_TYPES& rhs)
    {
      return rhs = static_cast<ECS::COMPONENT_TYPES>((static_cast<unsigned>(rhs) + 1));
    }
  
  } // namespace ECS
} // namespace GE