#ifndef RENDER_DATA_H
#define RENDER_DATA_H
#include <Graphics/Def/GraphicsTypes.h>

namespace Graphics::Rendering
{
  struct Transform
  {
    gVec3     scale;
    GLfloat   rotation;
    gVec3     pos;
  };

  /*!
   * \brief  struct containing info for rendering
   * .
   */
  struct RenderData 
  {
    gObjID mdl;         //!< model data (ALL MODELS MUST USE THIS)
    SpriteData sprite;  //!< sprite data handle (optional)
    Transform transform;//!< the transformation of the model
  };
};

#endif
