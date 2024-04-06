/*!*********************************************************************
\file   Mat4Utils.h
\author chengen.lau\@digipen.edu
\date   23-October-2023
\brief  Matrix 4x4 utility functions (only for floats and doubles)

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "GEM.h"

namespace GE
{
  namespace Math
  {
    /*!***********************************************************************
    \brief
      Takes in a Math::dMat4 and returns its inverse
    \param[in] mtx
      The matrix to inverse
    \return
      The inverse of the matrix
      Takes in a gpm::Mat4 and turns it into an identity matrix
    \param[in] mtx
      The matrix to store the result
    *************************************************************************/
    void MtxIdentity(Mat4& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat4 and turns it into an identity matrix
    \param[in] mtx
      The matrix to store the result
    *************************************************************************/
    void MtxIdentity(dMat4& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat4 and sets the result to the inverse of mtx
    \param[in] result
      The matrix to store the result
    \param[in] mtx
      The matrix to inverse
    *************************************************************************/
    void MtxInverse(Mat4& result, Mat4 const& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat4 and sets the result to the inverse of mtx
    \param[in] result
      The matrix to store the result
    \param[in] mtx
      The matrix to inverse
    *************************************************************************/
    void MtxInverse(dMat4& result, dMat4 const& mtx);

  }
}
