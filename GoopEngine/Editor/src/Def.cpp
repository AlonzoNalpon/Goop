#include <Def.h>
namespace debugger {
  void err_log(std::string const& msg) {
    std::cout << "ERROR: " << msg << std::endl;
  }

  void err_log_file(std::string const& msg) {
    std::cout << "ERROR: " << msg << " | in "
      << __FILE__ << ": " << __FUNCTION__ << " | line:" << __LINE__ << std::endl; // clowning. You're just printing this line and file
  }
}