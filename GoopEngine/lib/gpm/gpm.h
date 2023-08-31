#ifndef GOOP_MATH_H
#define GOOP_MATH_H
// may need to change includes in future
#include <math.h>
#include <type_traits>
#include <stdexcept>

namespace gpm
{
  // base Vec template
  template <unsigned N, typename T>
  struct Vec
  {
    // ensure only Vec2, 3, 4s are instantiated
    static_assert(!(N < 2 || N > 4) , "Vec<unsigned N, typename T>: only sizes 2, 3 and 4 are supported!");
  };

  typedef Vec<2, float> Vec2;
  typedef Vec<3, float> Vec3;
  typedef Vec<4, float> Vec4;

  // base Mat template
  template <unsigned Rows, unsigned Cols, typename T>
  struct Mat;

  typedef Mat<2, 2, float> Mat2;
  typedef Mat<3, 3, float> Mat3;
  typedef Mat<4, 4, float> Mat4;

} // namespace gpm

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "VecUtils.h"
#include "MatDefault.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"
#include "MatOperations.h"

#endif