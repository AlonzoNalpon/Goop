#ifndef DEF_H
#define DEF_H
#include <iostream>
#include <string>
using ubyte   = unsigned char;
using uint32  = unsigned int;
using i32     = int;
using f64     = double;
using f32     = float;
using wchar   = wchar_t; // word (2 byte) chars

namespace debugger {
  void err_log(std::string const& msg) {
    std::cout << "ERROR: " << msg << std::endl;
  }

  void err_log_file(std::string const& msg) {
    std::cout << "ERROR: " << msg << " | in "
      << __FILE__ << ": " << __FUNCTION__ << " | line:" << __LINE__ << std::endl;
  }
}

#define ERR_LOG( msg ) debugger::err_log(msg) 
#define ERR_LOG_FILE( msg ) debugger::err_log_file(msg)

#endif