#ifndef GOOP_MATH_H
#define GOOP_MATH_H
// may need to change includes in future
#include <math.h>
#include <type_traits>
#include <stdexcept>

namespace GE
{
  namespace Math
  {
    using size_type = unsigned;

    // base Vec template
    template <unsigned N, typename T>
    struct Vec
    {
        // ensure only Vec2, 3, 4s are instantiated
        static_assert(!(N < 2 || N > 4), "Vec<unsigned N, typename T>: only sizes 2, 3 and 4 are supported!");
    };

    using Vec2 = Vec<2, float>;
    using Vec3 = Vec<3, float>;
    using Vec4 = Vec<4, float>;
    using dVec2 = Vec<2, double>;
    using dVec3 = Vec<3, double>;
    using dVec4 = Vec<4, double>;

    // base Mat template
    template <unsigned Rows, unsigned Cols, typename T>
    struct Mat;

    using Mat2 = Mat<2, 2, float>;
    using Mat3 = Mat<3, 3, float>;
    using Mat4 = Mat<4, 4, float>;
    using dMat2 = Mat<2, 2, double>;
    using dMat3 = Mat<3, 3, double>;
    using dMat4 = Mat<4, 4, double>;
  }
} // namespace GE

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "VecUtils.h"
#include "MatDefault.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"
//#include "MatUtils.h"

#endif