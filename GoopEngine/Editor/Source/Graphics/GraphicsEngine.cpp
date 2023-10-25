/*!*********************************************************************
\file   GraphicsEngine.cpp
\author a.nalpon@digipen.edu
\date   24-September-2023
\brief  This file contains the implementation of the graphics engine class.
        This class houses everything required to render an opengl
        framebuffer. ImGui is not included in this process.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/GraphicsEngine.h>
#include <AssetManager/AssetManager.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FrameRateController/FrameRateController.h>
#include <SpriteAnimation/SpriteAnimGenerator.h>
//#define OGL_ERR_CALLBACK
  
namespace Graphics {
  
namespace {
  struct 
  {
    double currTime;
    gObjID animID;
    u32 currFrame;

  }testAnim;
}
#ifdef OGL_ERR_CALLBACK
  void GLAPIENTRY glDebugCallback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
    // Print the message to the console
    std::cerr << "OpenGL Message: " << message << std::endl;
}
#endif
  // Typedefs
  
  GraphicsEngine::GraphicsEngine() : m_vpWidth{}, m_vpHeight{}, m_ar{}, 
    m_renderer{ m_models, m_textureManager, m_shaders, m_fontManager, m_vpWidth, m_vpHeight }
  {
  }

  GraphicsEngine::~GraphicsEngine()
  {
  }

  void GraphicsEngine::Init(Colorf clearColor, GLint w, GLint h)
  {
#ifdef OGL_ERR_CALLBACK
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glDebugCallback, nullptr);
#endif
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glViewport(0, 0, w, h);
    m_vpWidth = w;
    m_vpHeight = h;
    m_ar = static_cast<GLfloat>(m_vpWidth) / m_vpHeight;
    
    InitFrameBuffer();
    // Initialize font manager
    m_fontManager.Init();
#pragma region SHADER_MDL_INIT
    auto shaderPathOpt = GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("ShaderPath");
    if (!shaderPathOpt) {
      throw 1; // lmao
    }
    m_spriteQuadMdl = GenerateQuad();
    m_lineMdl       = GenerateLine();
    ShaderInitCont spriteShaders{ { GL_VERTEX_SHADER, "sprite.vert" }, {GL_FRAGMENT_SHADER, "sprite.frag"}};
    ShaderInitCont debugLineShaders{ { GL_VERTEX_SHADER, "debug_line.vert" }, {GL_FRAGMENT_SHADER, "debug_line.frag"} };

    m_spriteQuadMdl.shader = CreateShader(spriteShaders, "sprite");
    // Adding the basic shader for rendering lines etc...
    m_lineMdl.shader       = CreateShader(debugLineShaders, "debug_line");
    m_models.emplace_back(m_spriteQuadMdl);

    //Initialize renderer with a camera
    {
      Rendering::Camera orthoCam{ {0.f,0.f,3.f},                          // pos
                                  {},                                     // target
                                  {.0f, 1.f, 0.f},                        // up vector
                                  -w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f,   // left right bottom top
                                  0.1f, 1000.f };                         // near and far z planes
      m_renderer.Init(orthoCam, m_models.size());
    }

    m_models.emplace_back(m_lineMdl);
#pragma endregion


    // THESE ARE IMPORTANT TO HAVE
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void GraphicsEngine::InitFrameBuffer()
  {
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);


    glGenTextures(1, &m_renderTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_vpWidth, m_vpHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      // Handle error
    }
  }

  GLuint GraphicsEngine::GetRenderTexture()
  {
    return m_renderTexture;
  }

  void GraphicsEngine::ClearBuffer()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void GraphicsEngine::Draw()
  {
#if 0
#pragma region UPDATE BLOCK
    double dt{ GE::FPS::FrameRateController::GetInstance().GetDeltaTime() };
#pragma endregion
    
    // use this for reference on animation inner workings
    testAnim.currTime += dt;
    auto anim = m_animManager.GetAnim(testAnim.animID);
    if (testAnim.currTime >= anim.speed) {
      testAnim.currTime -= anim.speed;
      if (++testAnim.currFrame == anim.frames.size())
        testAnim.currFrame = 0;
    }
    SpriteData spriteData{ anim.frames[testAnim.currFrame], anim.texture };
    constexpr f32 SCALE{ 1000.f };
    Rendering::Transform xform{ {SCALE,SCALE,SCALE}, 0.f, {400.f, 0.f, 0.f} };
    m_renderer.RenderObject(0, spriteData, xform);
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    ClearBuffer();
    glViewport(0, 0, m_vpWidth, m_vpHeight);
    m_renderer.Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  Model GraphicsEngine::GenerateQuad()
  {
    Model retval{};
    // Order of the quad's vtx data: bottom left, bottom right, top left, top right
    Colorf const WHITE{ 1.f, 1.f, 1.f, 1.f };
    GL_Data data
    {
      {{ -0.5, -0.5, .0 }, { 0.5, -0.5, .0 },  { -0.5, 0.5, .0}, { 0.5, 0.5, .0 }}, // the position data of a quad
      { WHITE,          WHITE,          WHITE,        WHITE },                      // the color data of the quad
      { {0.f, 0.f},     {1.f, 0.f},     {0.f, 1.f},   {1.f, 1.f} },                 // the texture data of the quad
    };


    GLuint vbo_hdl{}; // vertex buffer object handle
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, data.DataSize(), nullptr, GL_DYNAMIC_STORAGE_BIT); // STATIC MODELS

    // Assigning data ...
    GLsizeiptr data_offset{};
    // Position data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.PosDataSize(), data.pos_vtx.data());
    data_offset += data.PosDataSize(); // add the offset of the position vtx size
    // Color data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.ClrDataSize(), data.clr_vtx.data());
    data_offset += data.ClrDataSize(); // add the offset of the position vtx size
    // Tex data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.TexDataSize(), data.tex_vtx.data());

    data_offset = 0;
    GLuint vaoid{};
    glCreateVertexArrays(1, &vaoid);

    // Position data  LAYOUT LOCATION: 0
    glEnableVertexArrayAttrib(vaoid, 0);  // attribute 0 for pos
    glVertexArrayVertexBuffer(vaoid, 0, vbo_hdl, data_offset, static_cast<GLsizei>(data.PosTypeSize()));
    glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, 0); // 3 values of type: float not normalized
    glVertexArrayAttribBinding(vaoid, 0, 0); // attrib idx 0: binding idx 0
    data_offset += data.PosDataSize(); // add the size of all position vertices

    // Color data     LAYOUT LOCATION: 1
    glEnableVertexArrayAttrib(vaoid, 1); // attrib 1 for color
    glVertexArrayVertexBuffer(vaoid, 1, vbo_hdl, data_offset, static_cast<GLsizei>(data.ClrTypeSize()));
    glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0); // 3 values of type: float not normalized
    glVertexArrayAttribBinding(vaoid, 1, 1); // attrib idx 1: binding idx 1
    data_offset += data.ClrDataSize(); // add the size of all color vertices

    // Texture data   LAYOUT LOCATION: 2
    glEnableVertexArrayAttrib(vaoid, 2); // attrib 2 for tex coordinates
    glVertexArrayVertexBuffer(vaoid, 2, vbo_hdl, data_offset, static_cast<GLsizei>(data.TexTypeSize()));
    glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, 0); // 2 values of type: float not normalized
    glVertexArrayAttribBinding(vaoid, 2, 2); // attrib idx 2: binding idx 2

    // We won't be using indices for the quad ...
    glBindVertexArray(0); // All done. Unbind vertex array

    retval.draw_cnt = 4;
    retval.primitive_type = GL_TRIANGLE_STRIP;
    retval.primitive_cnt = 2; // not using this
    retval.vaoid = vaoid;
    return retval;
  }

  Model GraphicsEngine::GenerateLine()
  {
    Model retval{};
    // Order of the quad's vtx data: bottom left, bottom right, top left, top right
    Colorf const WHITE{ 1.f, 1.f, 1.f, 1.f };
    GL_Data data
    {
      {{ -0.5, 0.f, .0 }, { 0.5, 0.f, .0 }}, // the position data of a quad
      { WHITE,          WHITE},                      // the color data of the quad
      {}
    };


    GLuint vbo_hdl{}; // vertex buffer object handle
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, data.DataSize(), nullptr, GL_DYNAMIC_STORAGE_BIT); // STATIC MODELS

    // Assigning data ...
    GLsizeiptr data_offset{};
    // Position data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.PosDataSize(), data.pos_vtx.data());
    data_offset += data.PosDataSize(); // add the offset of the position vtx size
    // Color data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.ClrDataSize(), data.clr_vtx.data());

    data_offset = 0;
    GLuint vaoid{};
    glCreateVertexArrays(1, &vaoid);

    // Position data  LAYOUT LOCATION: 0
    glEnableVertexArrayAttrib(vaoid, 0);  // attribute 0 for pos
    glVertexArrayVertexBuffer(vaoid, 0, vbo_hdl, data_offset, static_cast<GLsizei>(data.PosTypeSize()));
    glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, 0); // 3 values of type: float not normalized
    glVertexArrayAttribBinding(vaoid, 0, 0); // attrib idx 0: binding idx 0
    data_offset += data.PosDataSize(); // add the size of all position vertices

    // Color data     LAYOUT LOCATION: 1
    glEnableVertexArrayAttrib(vaoid, 1); // attrib 1 for color
    glVertexArrayVertexBuffer(vaoid, 1, vbo_hdl, data_offset, static_cast<GLsizei>(data.ClrTypeSize()));
    glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0); // 3 values of type: float not normalized
    glVertexArrayAttribBinding(vaoid, 1, 1); // attrib idx 1: binding idx 1
    data_offset += data.ClrDataSize(); // add the size of all color vertices

    // We won't be using indices for the quad ...
    glBindVertexArray(0); // All done. Unbind vertex array

    retval.draw_cnt = 2;
    retval.primitive_type = GL_LINES;
    retval.primitive_cnt = 1; // not using this
    retval.vaoid = vaoid;
    return retval;
  }

  gObjID GraphicsEngine::GetShaderPgm(std::string const& pgmName)
  {
    // Find the shader
    ShaderLT::const_iterator res{ m_shaderLT.find(pgmName) };

    // If we cannot find the shader handle, return 0
    if (res == m_shaderLT.end())
      return 0;
    return res->second; // return the handle
  }

  GLuint GraphicsEngine::CreateShader(ShaderInitCont const& container, std::string const& name)
  {
    ShaderProgram shdrPgm;
    shdrPgm.CompileLinkValidate(container); 

    ShaderLT::const_iterator res{ m_shaderLT.find(name) };

    // If we find a conflicting name, print an error
    if (res != m_shaderLT.end())
    {
      std::string errorStr{ "A shader program of this name already exists: " };
      errorStr += name;
      std::cout << errorStr << std::endl;
      return 0;
    }

    // Now we add the shader and its associated name as the key
    m_shaderLT.emplace(name, shdrPgm.GetHandle());

    return shdrPgm.GetHandle(); // return the handle
  }

  Rendering::Renderer& GraphicsEngine::GetRenderer()
  {
    return m_renderer;
  }

  gObjID GraphicsEngine::InitTexture(std::string const& name, GE::Assets::ImageData const& imageData)
  {
    // Now use textures
    unsigned char* raw_image = imageData.GetData();
    GLsizei width{ static_cast<GLsizei>(imageData.GetWidth()) };
    GLsizei height{ static_cast<GLsizei>(imageData.GetHeight()) };
    return m_textureManager.AddTexture(name, width, height, raw_image);
  }

  gObjID GraphicsEngine::CreateAnimation(std::string const& name, GLuint slices, GLuint stacks, GLuint frames,
    f64 speed, u32 flags,  gObjID textureID)
  {
    Texture const& tex  { m_textureManager.GetTexture(textureID) };
    return m_animManager.CreateAnim(SpriteAnimGenerator::GenerateAnimData(slices, stacks, frames, tex.width, tex.height
    , speed, flags, textureID), name);
  }

  gObjID GraphicsEngine::GetModel()
  {
    return gObjID{};
  }

  GLint GraphicsEngine::GetVPWidth()
  {
    return m_vpWidth;
  }

  GLint GraphicsEngine::GetVPHeight()
  {
    return m_vpHeight;
  }

  gVec2 GraphicsEngine::ScreenToWS(gVec2 const& mousePos)
  {
    GLfloat const halfVpW{ m_vpWidth * 0.5f }, halfVpH{ m_vpHeight * 0.5f };
    // translate the mouse position to the range [-0.5,0.5]
    gVec2 wsPos{ (mousePos.x - halfVpW) / m_vpWidth , (mousePos.y - halfVpH) / m_vpHeight };
    gVec2 const camDims{ m_renderer.GetCamera().frame_dims };
    gVec2 const camPos{ m_renderer.GetCamera().position };
    // Now we scale based on camera dimensions

    wsPos.x *= camDims.x;
    wsPos.y *= camDims.y;
    wsPos += camPos;
    return wsPos;
  }

  void GraphicsEngine::DrawLine(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf clr)
  {
    m_renderer.RenderLineDebug(startPt, endPt, clr);
  }

}