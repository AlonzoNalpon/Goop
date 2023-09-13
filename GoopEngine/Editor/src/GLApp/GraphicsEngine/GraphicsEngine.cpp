#include <GLApp/Graphics/GraphicsEngine.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#define OGL_ERR_CALLBACK
namespace Graphics {
#ifdef OGL_ERR_CALLBACK
  void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    // Print the error message to the console
    std::cerr << "OpenGL Error: " << message << std::endl;
}
#endif

  GraphicsEngine::GraphicsEngine()
  {
  }

  GraphicsEngine::~GraphicsEngine()
  {
  }

  void GraphicsEngine::Init(Color clearColor)
  {
#ifdef OGL_ERR_CALLBACK
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glDebugCallback, nullptr);
#endif
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glViewport(0, 0, 800, 800);
    m_spriteQuad = GenerateQuad();
  }
  Model GraphicsEngine::GenerateQuad()
  {
    Model retval{};
    // Order of the quad's vtx data: bottom left, bottom right, top left, top right
    Color const WHITE{ 1.f, 1.f, 1.f };
    GL_Data data
    {
      { { -0.5, -0.5 }, { 0.5, -0.5 },  { -0.5, 0.5}, { 0.5, 0.5 } }, // the position data of a quad
      { WHITE,          WHITE,          WHITE,        WHITE }, // the color data of the quad
      { {0.f, 0.f},     {1.f, 0.f},     {0.f, 1.f},   {1.f, 1.f} }, // the texture data of the quad
    };

    GLuint vbo_hdl{}; // vertex buffer object handle
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, data.DataSize(), nullptr, GL_NONE); // STATIC MODELS

    // Assigning data ...
    GLsizeiptr data_offset{};
    // Position data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.PosDataSize(), data.pos_vtx.data());
    data_offset += data.PosDataSize(); // add the offset of the position vtx size
    // Color data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.ClrDataSize(), data.clr_vtx.data());
    data_offset += data.ClrDataSize(); // add the offset of the position vtx size
    // Tex data ...
    glNamedBufferSubData(vbo_hdl, data_offset, data.TexDataSize(), data.clr_vtx.data());

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

  void GraphicsEngine::Draw(Model const&)
  {

  }

  GLuint GraphicsEngine::GetShader(std::string const& fileName)
  {
    using ShaderLT = std::map<std::string const, GLuint>;
    // Find the shader
    ShaderLT::const_iterator res{ m_shaderLT.find(fileName) };

    // If we cannot find the shader handle, make a new program
    if (res == m_shaderLT.end()) 
    {

    }
    return GLuint();
  }
}