/*!*********************************************************************
\file   GEM.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Main header file for templated Math Library. Supports Vectors 
        of size 2, 3 and 4, and Matrices of any dimensions M*N within 
        2-4 of any data type. Contains all relevant typedefs and 
        includes for all classes involved. 

        For more information on usage, refer to Math/GEM_READ_ME.txt
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_MATH_H
#define GOOP_MATH_H

// may need to change includes in future
#define _USE_MATH_DEFINES
#include <math.h>
#include <type_traits>
#include <stdexcept>
#include <sstream>
#include <string>
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

namespace GE
{
  namespace Math
  {
    inline constexpr double PI{ 3.1415926535897932384626433 };

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

    #ifdef _DEBUG
    // << operator overload for printing Vectors to ostream
    template <unsigned N, typename T>
    std::ostream& operator<<(std::ostream& os, Vec<N, T> const& vec)
    {
      os << "(";
      for (unsigned i{}; i < N; ++i)
      {
        os << vec[i] << ((i == N - 1) ? ")" : ",");
      }
      return os;
    }

    // << operator overload for printing Matrices to ostream
    template <unsigned N, unsigned M, typename T>
    std::ostream& operator<<(std::ostream& os, Mat<N, M, T> const& mat)
    {
      os << std::setprecision(2) << std::fixed;
      for (unsigned i{}; i < N; ++i)
      {
        for (unsigned j{}; j < M; ++j)
        {
          os << mat[i][j] << (j == M - 1 ? "\n" : " ");
        }
      }
      return os;
    }
    #endif
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
#include "MatUtils.h"
#include "Mat3Utils.h"
#include "Mat4Utils.h"

#endif