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
      GE::ECS::Entity entityVal{ ECS::INVALID_ID }; //!< entity ID of element
      GLuint  spriteID{};         //!< the id of sprite to render if used
      GLuint  defaultSpriteID{};  //!< the default sprite ID to render
      bool used{}; //!< is the slot used?
    };

    std::vector<CardHolderEntry> elements;
  };
}

#endif
