#ifndef TEXT_RENDER_SYSTEM_H
#define TEXT_RENDER_SYSTEM_H
#include <ECS/System/System.h>
#include <Component/Text.h>
namespace GE::Systems {
  class TextRenderSystem : public GE::ECS::System
  {
  public:
    virtual void LateUpdate();
  };
}

#endif
