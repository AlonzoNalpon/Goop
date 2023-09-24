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
    Transform(GLfloat scale, GLfloat rot, gVec3 pos) : 
      scale{ scale, scale, scale }, rotation{ rot }, pos{pos} {}
    Transform(gVec3 const&scale, GLfloat rot, gVec3 const&pos) :
      scale{ scale }, rotation{ rot }, pos{ pos } {}
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
