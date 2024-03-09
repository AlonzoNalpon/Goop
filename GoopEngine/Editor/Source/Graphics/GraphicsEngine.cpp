/*!*********************************************************************
\file   GraphicsEngine.cpp
\author a.nalpon\@digipen.edu
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
  
namespace GE::Graphics {
  
namespace {
  struct 
  {
    double currTime;
    gObjID animID;
    u32 currFrame;

  }testAnim;
}
//#define OGL_ERR_CALLBACK
#ifdef OGL_ERR_CALLBACK
  void GLAPIENTRY glDebugCallback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
    // Print the message to the console
    std::cerr << "OpenGL Message: " << message << std::endl;
}
#endif
  // Typedefs
  
  GraphicsEngine::GraphicsEngine() : m_vpWidth{}, m_vpHeight{}, m_ar{}, 
    m_renderer{ m_models, m_textureManager, m_shaders, m_fontManager, m_vpWidth, m_vpHeight, m_frameBuffers }
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
    

#pragma region SHADER_MDL_INIT
    std::string shaderPathOpt = GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("ShaderPath");
    if (shaderPathOpt.empty()) 
    {
      throw GE::Debug::Exception<GraphicsEngine>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Invalid shader path"));
    }
    m_spriteQuadMdl = GenerateQuad();
    m_renderQuad    = GenerateQuad(1.f, 1.f); // a render quad covering the full screen!
    m_fontMdl = GenerateFontMdl();

    // INITIALIZING ALL SHADERS
    ShaderInitCont spriteShaders{ { GL_VERTEX_SHADER, "sprite.vert" }, {GL_FRAGMENT_SHADER, "sprite.frag"}};
    ShaderInitCont finalPassShaders{ { GL_VERTEX_SHADER, "final_pass.vert" }, {GL_FRAGMENT_SHADER, "final_pass.frag"} };
    ShaderInitCont fontShaders{ { GL_VERTEX_SHADER, "font.vert" }, {GL_FRAGMENT_SHADER, "font.frag"} };

    m_spriteQuadMdl.shader = CreateShader(spriteShaders, "sprite");
    m_renderQuad.shader    = CreateShader(finalPassShaders, "finalPass");
    m_fontMdl.shader       = CreateShader(fontShaders, "font");
    // Adding the basic shader for rendering lines etc...
    m_models.emplace_back(m_spriteQuadMdl);
#ifndef NO_IMGUI
    m_lineMdl       = GenerateLine();
    ShaderInitCont debugLineShaders{ { GL_VERTEX_SHADER, "debug_line.vert" }, {GL_FRAGMENT_SHADER, "debug_line.frag"} };
    m_lineMdl.shader       = CreateShader(debugLineShaders, "debug_line");
    m_models.emplace_back(m_lineMdl); // always load line model last! for renderer init
#endif

    //Initialize renderer with a camera: WILL BE REMOVED WHEN GAME CAMERA COMPONENTS ARE COMPLETED
    {
      Rendering::Camera orthoCam{ {0.f,0.f,1000.1f},                          // pos
                                  {},                                     // target
                                  {.0f, 1.f, 0.f},                        // up vector
                                  -w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f,   // left right bottom top
                                  0.1f, 2000.1f };                         // near and far z planes
      m_renderer.Init(orthoCam, m_models.size()-1); // line model index
    }
#pragma endregion

// compiler directive To be removed in future if engine does not need
#ifdef IMGUI_DISABLE
    CreateFrameBuffer(w, h);
#endif

    // Load font model and shader
    m_fontManager.Init(m_fontMdl.shader, m_fontMdl.vaoid);

    // THESE ARE IMPORTANT TO HAVE
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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

  void GraphicsEngine::SetDim(int width, int height)
  {
    m_vpWidth = width;
    m_vpHeight = height;
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
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    m_renderer.PreRenderSetup();
    for (auto& fbInfo : m_frameBuffers)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, fbInfo.second.frameBuffer);
      ClearBuffer();
      glViewport(0, 0, fbInfo.second.vpDims.x, fbInfo.second.vpDims.y);
      fbInfo.second.camera.CalculateViewProjMtx(); // Update camera
      m_renderer.Draw(fbInfo.second.camera);
    }
    m_renderer.ClearRenderContainers();
  #if 0 // BACKUP FOR MERGE
    m_renderer.Draw();
    auto marchID = m_fontManager.GetFontID("Marchesa");
    auto reyesID = m_fontManager.GetFontID("Reyes");

    m_renderer.DrawFontObj("you're", {}, gVec2{ FONT_SCALE ,FONT_SCALE }, { 0.5f, 0.f, 0.f }, marchID);
    m_renderer.DrawFontObj("next", { 0.f, -50.f }, gVec2{ FONT_SCALE ,FONT_SCALE }, { 0.8f, 0.2f, 0.f }, reyesID);
    #endif
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void GraphicsEngine::RenderToScreen(gObjID framebufferID)
  {
    auto const& fbInfo = m_frameBuffers[framebufferID];
    glUseProgram(m_renderQuad.shader); // USE SHADER PROGRAM
    glBindVertexArray(m_renderQuad.vaoid); // bind vertex array object to draw
    glBindTextureUnit(7, fbInfo.renderTexture);
    glTextureParameteri(fbInfo.renderTexture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(fbInfo.renderTexture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glDrawArrays(m_renderQuad.primitive_type, 0, m_renderQuad.draw_cnt);
    glBindTextureUnit(7, 0);
    glBindVertexArray(0);         // unbind vertex array object
    glUseProgram(0);
  }

  Model GraphicsEngine::GenerateQuad(GLfloat width, GLfloat height)
  {
    Model retval{};
    // Order of the quad's vtx data: bottom left, bottom right, top left, top right
    Colorf const WHITE{ 1.f, 1.f, 1.f, 1.f };
    GL_Data data
    {
      {{ -width, -height, .0 }, { width, -height, .0 },  { -width, height, .0}, { width, height, .0 }}, // the position data of a quad
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

  Model GraphicsEngine::GenerateFontMdl()
  {
    Model retval{};
    GLuint vbo_hdl{}, vaoid{}; // vertex buffer object handle

    glCreateVertexArrays(1, &vaoid);
    glBindVertexArray(vaoid);

    glCreateBuffers(1, &vbo_hdl); // generate vbo
    constexpr GLsizeiptr VERTICES_COUNT{ 6 };
    // allocate uninitialized memory for vbo (data to be set for each char later on)
    glNamedBufferData(vbo_hdl, sizeof(float) * VERTICES_COUNT * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexArrayAttrib(vaoid, 0);
    glVertexArrayAttribFormat(vaoid, 0, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 0, 0);

    // Binding vbo to vao
    glVertexArrayVertexBuffer(vaoid, 0, vbo_hdl, 0, 4 * sizeof(GLfloat));

    // We won't be using indices for the quad ...
    glBindVertexArray(0); // All done. Unbind vertex array

    retval.vaoid = vaoid;
    return retval;
  }

  SpriteAnimationManager& GraphicsEngine::GetAnimManager()
  {
    return m_animManager;
  }

  void GraphicsEngine::DestroyTexture(GLuint texture)
  {
    m_textureManager.DestroyTexture(texture);
  }

  void GraphicsEngine::FreeTexturesAndFonts()
  {
    m_fontManager.FreeFonts();
    m_textureManager.FreeTextures();
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
      GE::Debug::ErrorLogger::GetInstance().LogError(errorStr);
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

  GLuint GraphicsEngine::InitTexture(std::string const& name, GE::Assets::ImageData const& imageData)
  {
    // Now use textures
    unsigned char* raw_image = imageData.GetData();
    GLsizei width{ static_cast<GLsizei>(imageData.GetWidth()) };
    GLsizei height{ static_cast<GLsizei>(imageData.GetHeight()) };
    return m_textureManager.AddTexture(name, width, height, raw_image);
  }

  gObjID GraphicsEngine::CreateAnimation(std::string const& name, GLuint slices, GLuint stacks, GLuint frames,
    f64 speed, u32 flags,  GLuint textureID)
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

  gVec2 GraphicsEngine::ScreenToWS(gVec2 const& mousePos, gObjID frameBuffer)
  {
    auto const& fbInfo{ m_frameBuffers.at(frameBuffer) };     // frame buffer info
    gVec2 const& fbFrameDims{ fbInfo.camera.GetFrameDims() }; // framebuffer dimensions
    gVec3 const& fbCamPos{ fbInfo.camera.GetPos() };          // cameraposition

    GLfloat const halfVpW{ fbInfo.vpDims.x * 0.5f }, halfVpH{ fbInfo.vpDims.y * 0.5f };
    // translate the mouse position to the range [-0.5,0.5]
    gVec2 wsPos{ (mousePos.x - halfVpW) / fbInfo.vpDims.x , (mousePos.y - halfVpH) / fbInfo.vpDims.y };

    // Now we scale based on camera dimensions, and translate based on position!
    wsPos.x *= fbFrameDims.x;
    wsPos.y *= fbFrameDims.y;
    wsPos.x += fbCamPos.x;
    wsPos.y += fbCamPos.y;
    return wsPos;
  }

  gObjID GraphicsEngine::CreateFrameBuffer(GLint width, GLint height)
  {
    Rendering::FrameBufferInfo newFB{ {m_frameBuffers.size()}, {}, {},
      // The camera currently covers 1000 to -1000
    Rendering::Camera{ {0.f,0.f,1000.1f},                          // pos
    {},                                                           // target
    {.0f, 1.f, 0.f},                                              // up vector
    -width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f,   // left right bottom top
    0.1f, 2000.9f } , 
      {width, height} // viewport dimensions in pixels
  };
    glGenFramebuffers(1, &newFB.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, newFB.frameBuffer);


    glGenTextures(1, &newFB.renderTexture);
    glBindTexture(GL_TEXTURE_2D, newFB.renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newFB.renderTexture, 0);


    gObjID fbID = newFB.objID;
    m_frameBuffers.emplace(fbID, newFB);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw GE::Debug::Exception<GraphicsEngine>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Created framebuffer failed completeness test!"));
    }
      return fbID;
  }

  Rendering::FrameBufferInfo& GraphicsEngine::GetFrameBuffer(gObjID id)
  {
    auto it = m_frameBuffers.find(id);
    if (it == m_frameBuffers.end())
    {
      std::string errMsg{ "Framebuffer does not exist! ID: " };
      errMsg += std::to_string(id);
      throw GE::Debug::Exception<GraphicsEngine>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg(errMsg));
    }
    return it->second;
  }

  void GraphicsEngine::DrawLine(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf clr)
  {
    m_renderer.RenderLineDebug(startPt, endPt, clr);
  }
}