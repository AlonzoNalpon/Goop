#ifndef DEF_H
#define DEF_H
#include <iostream>
#include <string>
#include <gl/glew.h>
using ubyte   = unsigned char;
using uChar   = unsigned char;
using uint32  = unsigned int;
using i32     = int;
using f64     = double;
using f32     = float;
using wchar   = wchar_t; // word (2 byte) chars
using u32     = unsigned int;
using u64     = unsigned long long;
namespace debugger {
  void err_log(std::string const& msg);

  void err_log_file(std::string const& msg);
}

#pragma region CONSTANTS
std::string const ASSETS_PATH{ "Assets/" };
std::string const SHADER_PATH{ ASSETS_PATH + "Shaders/" };
#pragma endregion

#define ERR_LOG( msg ) debugger::err_log(msg) 
#define ERR_LOG_FILE( msg ) debugger::err_log_file(msg)

#ifdef _DEBUG
// Alonzo: I am using this for additional checks to help debugging
// see SpriteAnimationManager.cpp for an example
#define INVALID_VALUE_CHECK 
#endif
#endif