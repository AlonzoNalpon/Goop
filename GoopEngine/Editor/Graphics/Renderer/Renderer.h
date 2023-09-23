#ifndef RENDERER_H
#define RENDERER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Def/RenderingHelpers.h>
#include <Graphics/Renderer/RenderData.h>
#include <Graphics/TextureManager.h>
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
    Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, ShaderCont const& shaderCont);
    void Init(size_t renderCallSize = 2048);
    void RenderObject(gObjID mdl, SpriteData const& sprite);
    //void RenderObject(gObjID mdl, gObjID sprite);
    void Draw();
  private:
    std::vector<RenderData> m_renderCalls;

    std::vector<Model> const&                     r_mdlContainer;
    TextureManager const&                         r_texManager;
    ShaderCont const&                             r_shaders;
  };
}
#endif
