/*!*********************************************************************
\file   Mat3Utils.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Matrix 3x3 utility functions (only for floats and doubles)
  
 
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
      Takes in a gpm::Mat3 and turns it into an identity matrix
    \param[in] mtx
      The matrix to store the result
    *************************************************************************/
    void MtxIdentity(Mat3& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and turns it into an identity matrix
    \param[in] mtx
      The matrix to store the result
    *************************************************************************/
    void MtxIdentity(dMat3& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::Mat3 and sets it to a scale matrix of x and y
    \param[in] mtx
      The matrix to store the result
    \param[in] x
      The x-component of the scale
    \param[in] y
      The y-component of the scale
    *************************************************************************/
    void MtxScale(Mat3& mtx, float x, float y);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and sets it to a scale matrix of x and y
    \param[in] mtx
      The matrix to store the result
    \param[in] x
      The x-component of the scale
    \param[in] y
      The y-component of the scale
    *************************************************************************/
    void MtxScale(dMat3& mtx, float x, float y);

    /*!***********************************************************************
    \brief
      Takes in a gpm::Mat3 and sets it to a rotation matrix of the given
      angle (counter-clockwise)
    \param[in] mtx
      The matrix to store the result
    \param[in] radians
      The angle in radians
    *************************************************************************/
    void MtxRotate(Mat3& mtx, float radians);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and sets it to a rotation matrix of the given
      angle (counter-clockwise)
    \param[in] mtx
      The matrix to store the result
    \param[in] radians
      The angle in radians
    *************************************************************************/
    void MtxRotate(dMat3& mtx, float radians);

    /*!***********************************************************************
    \brief
      Takes in a gpm::Mat3 and sets it to a translation matrix of x and y
    \param[in] mtx
      The matrix to store the result
    \param[in] x
      The x-component of the translate
    \param[in] y
      The y-component of the translate
    *************************************************************************/
    void MtxTranslate(Mat3& mtx, float x, float y);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and sets it to a translation matrix of x and y
    \param[in] mtx
      The matrix to store the result
    \param[in] x
      The x-component of the translate
    \param[in] y
      The y-component of the translate
    *************************************************************************/
    void MtxTranslate(dMat3& mtx, float x, float y);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and sets the result to the inverse of mtx
    \param[in] result
      The matrix to store the result
    \param[in] mtx
      The matrix to inverse
    *************************************************************************/
    void MtxInverse(Mat3& result, dMat3 const& mtx);

    /*!***********************************************************************
    \brief
      Takes in a gpm::dMat3 and sets the result to the inverse of mtx
    \param[in] result
      The matrix to store the result
    \param[in] mtx
      The matrix to inverse
    *************************************************************************/
    void MtxInverse(dMat3& result, dMat3 const& mtx);

  }
}
