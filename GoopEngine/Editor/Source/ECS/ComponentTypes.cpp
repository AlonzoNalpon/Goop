#include "ComponentTypes.h"

using namespace GE::ECS;

std::vector<std::string> GE::ECS::ComponentSignatureToString(ComponentSignature sig)
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
