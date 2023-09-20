#ifndef RENDERER_H
#define RENDERER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Def/RenderingHelpers.h>
#include <Graphics/Renderer/RenderData.h>
#include <Graphics/GraphicsEngine.h>
namespace Graphics::Renderer
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
  public:
    Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, Graphics::GraphicsEngine::ShaderCont const& shaderCont);
    void Init();
    void RenderObject(gObjID mdl, SpriteData const& sprite);
    //void RenderObject(gObjID mdl, gObjID sprite);
    void Draw();
  private:
    std::vector<RenderData> m_renderCalls;

    std::vector<Model> const&                     r_mdlContainer;
    TextureManager const&                         r_texManager;
    Graphics::GraphicsEngine::ShaderCont const&   r_shaders;
  };
}
#endif
