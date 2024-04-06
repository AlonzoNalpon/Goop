/*!*********************************************************************
\file   RenderData.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This class contains the definitions of rendering data structures
        for game objects. These objects will be passed to renderer.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef RENDER_DATA_H
#define RENDER_DATA_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Fonts/FontManager.h>
namespace GE::Graphics::Rendering
{
  /*!
   * \struct Transform
   * \brief  
   * Transform structure for model.
   */
  struct Transform
  {
    //gVec3     scale;      // deprecated
    //GLfloat   rotation;   // deprecated
    //gVec3     pos;        // deprecated

    //glm::mat4 mat;

    //Transform(GLfloat scale, GLfloat rot, gVec3 pos) : 
    //  scale{ scale, scale, scale }, rotation{ rot }, pos{pos} {}
    //Transform(gVec3 const&scale, GLfloat rot, gVec3 const&pos) :
    //  scale{ scale }, rotation{ rot }, pos{ pos } {}


  };

  enum class RenderDataTypes : u32
  {
    SPRITE,
    FONT,
    LINE
  };

  /*!
   * \struct RenderData
   * \brief  struct containing info for rendering
   * Dataset containing model information for rendering object.
   */
  struct SpriteRenderData 
  {
    //gObjID mdl;         //!< model data (ALL MODELS MUST USE THIS). It has shader stored within
    SpriteData sprite;  //!< sprite data handle (optional)
    glm::mat4 transform;//!< the transformation of the model
  };


  struct FontRenderData
  {
    gVec2                       position;
    GLfloat                     scale;
    std::string const&          str;
                 
    std::vector<GE::Graphics::Fonts::FontManager::FontLineInfo> const& newLines;
    Graphics::Colorf            clr;
    gObjID                      fontID;
  };

  /*!
   * \struct LineRenderData
   * \brief  
   * struct containing info for rendering lines.
   */
  struct LineRenderData
  {
    GE::Math::dVec2 startPt;
    GE::Math::dVec2 endPt;
    Colorf clr;
  };

  struct RenderData
  {
    RenderDataTypes type;
    size_t element;
    f32 zVal;
  };

  struct DepthComp
  {
    bool operator()(SpriteRenderData const& a, SpriteRenderData const& b)const
    {
      return a.transform[3][2] < b.transform[3][2];
    }
  };
};

#endif
