#ifndef CARD_HOLDER_SYSTEM_H
#define CARD_HOLDER_SYSTEM_H
#include <ECS/System/System.h>
#include <Component/CardHolder.h>
namespace GE::Systems {
  class CardHolderSystem : public GE::ECS::System
  {
  public:
    virtual void Update();
  };
}
#endif
