#ifndef CARD_HOLDER_H
#define CARD_HOLDER_H
#include <ECS/Entity/Entity.h>
#include <def.h>
#include <vector>
namespace GE::Component
{
  struct CardHolder
  {
    struct CardHolderEntry
    {
      GE::ECS::Entity elemEntity{ ECS::INVALID_ID }; //!< entity ID of holder element
      GE::ECS::Entity cardEntity{ ECS::INVALID_ID }; //!< entity ID of card
      GLuint  spriteID{};         //!< the id of sprite to render if used
      GLuint  defaultSpriteID{};  //!< the default sprite ID to render
      bool used{}; //!< is the slot used?
    };

    std::vector<CardHolderEntry> elements;
  };
}

#endif
