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
    // Print the error message to the console
    std::cerr << "OpenGL Message: " << message << std::endl;
}
#endif
  // Typedefs
  
  GraphicsEngine::GraphicsEngine() : m_vpWidth{}, m_vpHeight{}, m_ar{}, m_renderer{ m_models, m_textureManager, m_shaders }
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
    //Initialize renderer with a camera
    {
      Rendering::Camera orthoCam{ {0.f,0.f,3.f},                // pos
                                  {},                         // target
                                  {.0f, 1.f, 0.f},              // up vector
                                  -w*0.5f, w*0.5f, -h * 0.5f, h * 0.5f,  // left right bottom top
                                  0.1f, 10.f };               // near and far z planes
      m_renderer.Init(orthoCam);
    }
#pragma region SHADER_MDL_INIT


    m_spriteQuadMdl = GenerateQuad();
    m_lineMdl       = GenerateLine();
    ShaderInitCont spriteShaders{ { GL_VERTEX_SHADER, "sprite.vert" }, {GL_FRAGMENT_SHADER, "sprite.frag"}};
    ShaderInitCont debugLineShaders{ { GL_VERTEX_SHADER, "debug_line.vert" }, {GL_FRAGMENT_SHADER, "debug_line.frag"} };

    m_spriteQuadMdl.shader = CreateShader(spriteShaders, "sprite");
    // Adding the basic shader for rendering lines etc...
    m_lineMdl.shader       = CreateShader(debugLineShaders, "debug_line");
    m_models.emplace_back(m_spriteQuadMdl);
    m_models.emplace_back(m_lineMdl);
#pragma endregion

    

#pragma region TEXTURE_TEST
    // Now use textures
    auto& assetManager{ GE::AssetManager::AssetManager::GetInstance() };
    GE::AssetManager::ImageData imageData = assetManager.GetData( ASSETS_PATH + "MineWorm.png");
    unsigned char* raw_image = imageData.GetData();
    GLsizei width{ static_cast<GLsizei>(imageData.GetWidth()) };
    GLsizei height{ static_cast<GLsizei>(imageData.GetHeight()) };
#pragma endregion

#pragma region SPRITE_ANIMATION_TEST
    testAnim.currTime = .0;
    gObjID texObjID{ m_textureManager.AddTexture(width, height, raw_image) };
    u32 animFlags{};
    animFlags |= SPRITE_ANIM_FLAGS::LOOPING; // this animation will loop
    testAnim.animID = m_animManager.CreateAnim(SpriteAnimGenerator::GenerateAnimData
    (6, 1, width, height, .1f, animFlags, texObjID), "MineWorm");
    
#pragma endregion

    // THESE ARE IMPORTANT TO HAVE
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void GraphicsEngine::Draw()
  {
    glClear(GL_COLOR_BUFFER_BIT);
#pragma region UPDATE BLOCK
    double dt{ GE::FPS::FrameRateController::GetInstance().GetDeltaTime() };
#pragma endregion
    // This block is just for reference
    // TODO: DELETE
    {

    //auto const& mdl{ m_models.front() };
    //
    //
    //glBindTextureUnit(6, testTexture.texture);
    //glTextureParameteri(testTexture.texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTextureParameteri(testTexture.texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    //glUseProgram(mdl.shader);
    //// Setting uniform variables
    //GLint const texPosLocation = 0;  // Layout location for uTexPos
    //GLint const texDimsLocation = 1; // Layout location for uTexDims

    //// Set the uniform values using the layout locations
    ////glUniform2f(texDimsLocation, 0.5f, 0.5f);
    //glUniform2f(texDimsLocation, 0.5f, 0.5f);
    //glUniform2f(texPosLocation, 0.5f, 0.5f);

    ////GLint tex_loc = glGetUniformLocation(mdl.shader, "uTex2d");
    ////glUniform1i(tex_loc, 6); // Binding to texture unit 0

    //glBindVertexArray(mdl.vaoid);
    ////glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt);
    //glDrawArrays(GL_LINES, 0, mdl.draw_cnt);
    //
    //glBindVertexArray(0);
    //glUseProgram(0);
    //glBindTextureUnit(6, 0);

    /*for (auto const&mdl : m_models)
    {
      GLint tex_loc = glGetUniformLocation(mdl.shader, "uTex2d");
      glUniform1i(tex_loc, 0);
      glBindVertexArray(mdl.vaoid);
      glUseProgram(mdl.shader);
      glBindTextureUnit(0, testTexture.texture);
      glDrawArrays(mdl.primitive_type, 0, mdl.draw_cnt);
      glBindTextureUnit(0, 0);
      glUseProgram(0);
      glBindVertexArray(0);
    }*/
    }

#if 0
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
    m_renderer.Draw();
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

  void GraphicsEngine::DrawMdl(Model const& mdl)
  {
    // not implemented yet
  }

  void GraphicsEngine::DrawMdl(Model const& mdl, SpriteData const& sprite)
  {

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
      ERR_LOG_FILE(errorStr);
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

  gObjID GraphicsEngine::GetModel()
  {
    return gObjID{};
  }

  void GraphicsEngine::DrawLine(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf clr)
  {

    glLineWidth(1.f);
    GraphicsEngine& gEngine{ GetInstance() };
    auto const& mdl{ gEngine.m_lineMdl };
    gVec2 newStart{ startPt.x / gEngine.m_vpWidth * 2, startPt.y / gEngine.m_vpHeight * 2};
    gVec2 newEnd{ endPt.x / gEngine.m_vpWidth * 2, endPt.y / gEngine.m_vpHeight * 2};
    //auto handle{ gEngine.m_shaders[gEngine.m_lineShdrPgm].GetHandle() };
    glBindVertexArray(mdl.vaoid);
    glUseProgram(gEngine.m_lineMdl.shader);

    // Setting uniform variables
    //GLint const endPtsULocation = 0;  // Layout location for endPoints
    //GLint const colorULocation = 1; // Layout location for uTexDims

    int endPtsULocation = glGetUniformLocation(gEngine.m_lineMdl.shader, "uEndPts");
    int colorULocation = glGetUniformLocation(gEngine.m_lineMdl.shader, "uColor");

    // Set the uniform values using the layout locations
    glUniform4f(endPtsULocation, newStart.x, newStart.y, newEnd.x, newEnd.y);
    glUniform3f(colorULocation, clr.r, clr.g, clr.b);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
    glUseProgram(0);
  }

}