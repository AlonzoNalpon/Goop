#include <GLApp/Graphics/ShaderProgram.h>
#include <Def.h>
#include <fstream>
#include <sstream>
namespace Graphics {

  GLboolean ShaderProgram::CompileFromString(GLenum shaderType, std::string const& shaderSrc)
  {
    if (m_pgmHandle <= 0) 
    {
      m_pgmHandle = glCreateProgram();
      if (0 == m_pgmHandle) 
      {
        ERR_LOG_FILE("Unable to create program handle");
        return GL_FALSE;
      } 
    }
    GLuint shaderHandle{};
    // Create shader based on type
    switch (shaderType)
    {
    case GL_VERTEX_SHADER: shaderHandle = glCreateShader(GL_VERTEX_SHADER); break;
    case GL_FRAGMENT_SHADER: shaderHandle = glCreateShader(GL_FRAGMENT_SHADER); break;
    case GL_GEOMETRY_SHADER: shaderHandle = glCreateShader(GL_GEOMETRY_SHADER); break;
    case GL_TESS_CONTROL_SHADER: shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER); break;
    case GL_TESS_EVALUATION_SHADER: shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
    default:
      // An unknown enumerated value specified!
      std::string errorStr{ "Incorrect shader type enum specified: " };
      errorStr += std::to_string(shaderType);
      ERR_LOG_FILE(errorStr);
    }
    GLchar const* shdrCode[]{ shaderSrc.c_str() };
    glShaderSource(shaderHandle, 1, shdrCode, NULL);

    // Compile shader
    glCompileShader(shaderHandle);

    GLint compResult{};
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compResult);
    if (GL_FALSE == compResult)
    {
      GLint logLen; // length of log string
      glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLen);
      std::string errorStr{ "Failure to compile shader: " }; // error string for debugger

      if (logLen > 0)
      {
        // Here we get the error information to add to error string
        GLchar* log = new GLchar[logLen]{};
        GLsizei writtenLogLen{};
        glGetShaderInfoLog(shaderHandle, logLen, &writtenLogLen, log);
        errorStr += log;
        delete[] log;
      }
      ERR_LOG_FILE(errorStr);
      return GL_FALSE; // return false after printing error
    }

    // The shader is successful if this point is reached
    glAttachShader(m_pgmHandle, shaderHandle);

    // TODO: Now delete the shader
    // glDeleteShader(shaderHandle);
    return GL_TRUE;
  }

  GLboolean ShaderProgram::CompileFromFile(GLenum shaderType, std::string const& filePath)
  {
    // ensure shader program has been made
    if (m_pgmHandle <= 0)
    {
      m_pgmHandle = glCreateProgram();
      if (0 == m_pgmHandle)
      {
        ERR_LOG_FILE("Unable to create program handle");
        return GL_FALSE;
      }
    }
    // Getting the shader file
    std::ifstream ifs{ SHADER_PATH + filePath };
    std::cout << SHADER_PATH + filePath << std::endl;
    if (!ifs)
    {
      std::string errorStr{ "Unable to open shader file: " };
      errorStr += filePath;
      ERR_LOG_FILE(errorStr);
      return GL_FALSE;
    }
    
    std::ostringstream buff{};
    buff << ifs.rdbuf();
    
    return CompileFromString(shaderType, buff.str());
  }

  GLboolean ShaderProgram::Link()
  {
    if (GL_TRUE == m_isLinked) 
    {
      return GL_TRUE;
    }
    if (m_pgmHandle <= 0) 
    {
      // NOTE: can log attempting to link when program is not created
      return GL_FALSE;
    }

    glLinkProgram(m_pgmHandle);

    // Verify link status
    GLint linkStatus{};
    glGetProgramiv(m_pgmHandle, GL_LINK_STATUS, &linkStatus);
    if (GL_FALSE == linkStatus)
    {
      GLint logLen{};
      std::string errorStr{"Failed to link shader program: "};

      if (logLen > 0)
      {
        // Here we get the error information to add to error string
        GLchar* log = new GLchar[logLen]{};
        GLsizei writtenLogLen{};
        glGetShaderInfoLog(m_pgmHandle, logLen, &writtenLogLen, log);
        errorStr += log;
        delete[] log;
      }
      ERR_LOG_FILE(errorStr);
      return GL_FALSE; // return false after printing error
    }
    
    return GLboolean();
  }

  GLboolean ShaderProgram::Validate()
  {
    if (m_pgmHandle <= 0 || m_isLinked == GL_FALSE) {
      return GL_FALSE;
    }

    glValidateProgram(m_pgmHandle);

    // verify validation status
    GLint linkStatus{};
    glGetProgramiv(m_pgmHandle, GL_VALIDATE_STATUS, &linkStatus);
    if (GL_FALSE == linkStatus)
    {
      GLint logLen{};
      std::string errorStr{ "Failed to validate shader program for current Opengl context: " };

      if (logLen > 0)
      {
        // Here we get the error information to add to error string
        GLchar* log = new GLchar[logLen]{};
        GLsizei writtenLogLen{};
        glGetShaderInfoLog(m_pgmHandle, logLen, &writtenLogLen, log);
        errorStr += log;
        delete[] log;
      }
      ERR_LOG_FILE(errorStr);
      return GL_FALSE; // return false after printing error
    }
    return GL_TRUE;
  }

  GLboolean ShaderProgram::CompileLinkValidate(ShaderCont const& cont)
  {
    for (auto const& elem : cont)
    {
      if (GL_FALSE == CompileFromFile(elem.first, elem.second.c_str()))
        return GL_FALSE;
    }
    if (GL_FALSE == Link())
      return GL_FALSE;
    if (GL_FALSE == Validate())
      return GL_FALSE;

    // Any additional printing goes here
    return GL_TRUE;
  }

  GLuint ShaderProgram::GetHandle() const
  {
      return m_pgmHandle;
  }

  void ShaderProgram::DeleteShdrPgm()
  {
    if (m_pgmHandle > 0)
      glDeleteProgram(m_pgmHandle);
  }
}
