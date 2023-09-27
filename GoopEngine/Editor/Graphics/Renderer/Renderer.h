#ifndef RENDERER_H
#define RENDERER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Def/RenderingHelpers.h>
#include <Graphics/Renderer/RenderData.h>
#include <Graphics/TextureManager.h>
#include <Graphics/Renderer/Camera.h>
#include <Graphics/Fonts/FontManager.h>
namespace Graphics::Rendering
{
  /*!
   * \class Renderer
   * \brief  
   * class to perform rendering for graphics engine.
   * \note In the future, render order will be a problem, especially with transparency
   * This class is meant to solve it with future implementations.
   * 
   * reduced OGL call optimizations should be made in this implementation
   */
  class Renderer {
    using ShaderLT = std::map<std::string const, gObjID>;
    using ShaderCont = std::vector<ShaderProgram>;
  public:
    Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, ShaderCont const& shaderCont
    , Fonts::FontManager const& fontManager);

    void Init(Camera const& camera, size_t renderCallSize = 2048);
    void RenderObject(gObjID mdl, SpriteData const& sprite, Transform const& transform);
    //void RenderObject(gObjID mdl, gObjID sprite);
    void Draw();
  private:
    // Private methods

    glm::mat4 CalculateTransform(gVec3 const& scale, GLfloat rotation, gVec3 const& pos) const;
    glm::mat4 CalculateTransform(Transform const& xForm) const;
  private:
    std::vector<RenderData> m_renderCalls;

    Camera                              m_camera;   //!< camera for rendering

    std::vector<Model> const&           r_mdlContainer;
    TextureManager const&               r_texManager;
    ShaderCont const&                   r_shaders;
    Fonts::FontManager const&           r_fontManager;
  };
}
#endif
