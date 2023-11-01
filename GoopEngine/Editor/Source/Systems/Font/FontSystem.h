#ifndef FONT_SYSTEM_H
#define FONT_SYSTEM_H
#include <ECS/System/System.h>
#include <Component/TextObject.h>
namespace GE::Systems {
  class FontSystem : public GE::ECS::System
  {
  public:
    virtual void Start();

    virtual void Update();

    virtual void OnDestroyed();
  };
}

#endif
