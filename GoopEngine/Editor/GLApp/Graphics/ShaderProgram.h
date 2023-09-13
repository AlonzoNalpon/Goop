#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <gl/glew.h>
#include <string>
#include <vector>
namespace Graphics {
  using ShaderCont = std::vector<std::pair<GLenum, std::string>>;
  
  /*!
   * \brief  
   * A shader program that contains a handle and its linking status.
   * It will not automatically delete the shader when destroyed. You must
   * manually call DeleteShdrPgm()
   */
  class ShaderProgram {
  public:
    ShaderProgram() : m_pgmHandle{}, m_isLinked{ GL_FALSE } {}

    GLboolean CompileFromString(GLenum shaderType, std::string const& shaderSrc);
    GLboolean CompileFromFile(GLenum shaderType, std::string const& filePath);
    GLboolean Link();
    GLboolean Validate();
    GLboolean CompileLinkValidate(ShaderCont const&);
    GLuint    GetHandle() const;
    void DeleteShdrPgm();
  private:
    GLuint    m_pgmHandle;    //!< shader program handle
    GLboolean m_isLinked;     //!< flag for program linked
  };
}

#endif