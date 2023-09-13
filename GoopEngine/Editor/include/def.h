#ifndef DEF_H
#define DEF_H
#include <iostream>
#include <string>
#include <gl/glew.h>
using ubyte   = unsigned char;
using uint32  = unsigned int;
using i32     = int;
using f64     = double;
using f32     = float;
using wchar   = wchar_t; // word (2 byte) chars

namespace debugger {
  void err_log(std::string const& msg);

  void err_log_file(std::string const& msg);
}

#pragma region CONSTANTS
std::string const SHADER_PATH{ "../Assets/Shaders/" };
#pragma endregion

#define ERR_LOG( msg ) debugger::err_log(msg) 
#define ERR_LOG_FILE( msg ) debugger::err_log_file(msg)

#endif