#ifndef CARD_HOLDER_ELEM_H
#define CARD_HOLDER_ELEM_H

#include <ECS/Entity/Entity.h>

namespace GE::Component
{

  struct CardHolderElem
  {
    ECS::Entity holder{ECS::INVALID_ID};
    size_t elemIdx{ECS::INVALID_ID}; // !< which element in the container of holder
  };
}

#endif
