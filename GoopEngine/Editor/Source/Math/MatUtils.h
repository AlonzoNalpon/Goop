/*!*********************************************************************
\file   MatUtils.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Matrix utility template functions
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_MAT_UTILS_H
#define GOOP_MAT_UTILS_H

namespace GE
{
  namespace Math
  {
    template <size_type N, size_type M, size_type P, typename T>
    Mat<N, P, T> operator*(Mat<N, M, T> const& lhs, Mat<M, P, T> const& rhs);

    /*!***********************************************************************
    \brief
      Takes in a gpm::Mat and sets the result to the transpose
    \param[in] result
      The matrix to store the result
    \param[in] mtx
      The matrix to inverse
    *************************************************************************/
    template <size_type N, size_type M, typename T>
    void MtxTranspose(Mat<N, M, T>& result, Mat<M, N, T> const& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::Mat3 returns its determinant
    \param[in] mtx
      The input matrix
    *************************************************************************/
    template <typename T>
    T MtxDeterminant(Mat<3, 3, T> const& mtx);

    #include "MatUtils.tpp"
  }
} // namespace GE

#endif