/*!*********************************************************************
\file   Mat3Utils.h
\author chengen.lau\@digipen.edu
\date   23-October-2023
\brief  Matrix 4x4 utility functions (only for floats and doubles)


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Mat4Utils.h"

void GE::Math::MtxIdentity(Mat4& mtx)
{
  mtx[0] = Mat4::ValueType(1.f, 0.f, 0.f, 0.f);
  mtx[1] = Mat4::ValueType(0.f, 1.f, 0.f, 0.f);
  mtx[2] = Mat4::ValueType(0.f, 0.f, 1.f, 0.f);
  mtx[3] = Mat4::ValueType(0.f, 0.f, 0.f, 1.f);
}

void GE::Math::MtxIdentity(dMat4& mtx)
{
  mtx[0] = dMat4::ValueType(1.0, 0.0, 0.0, 0.0);
  mtx[1] = dMat4::ValueType(0.0, 1.0, 0.0, 0.0);
  mtx[2] = dMat4::ValueType(0.0, 0.0, 1.0, 0.0);
  mtx[3] = dMat4::ValueType(0.0, 0.0, 0.0, 1.0);
}

void GE::Math::MtxInverse(Mat4& result, dMat4 const& mtx)
{
  double const det{ MtxDeterminant(mtx) };
  if (det == 0.0) { throw std::runtime_error("division by zero"); }

  // Calculate the adjugate of the input matrix
  dMat4 adjugate;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      // Calculate the cofactor for this element
      dMat3 submatrix;
      int sub_row = 0, sub_col = 0;
      for (int i = 0; i < 4; ++i) {
        if (i == row) continue;
        for (int j = 0; j < 4; ++j) {
          if (j == col) continue;
          submatrix[sub_row][sub_col] = mtx[i][j];
          sub_col++;
        }
        sub_row++;
      }
      double cofactor = ((row + col) % 2 == 0) ? MtxDeterminant(submatrix) : -MtxDeterminant(submatrix);

      // Fill in the adjugate matrix
      adjugate[row][col] = cofactor;
    }
  }

  // Calculate the inverse by dividing the adjugate by the determinant
  double invDet = 1.0 / det;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      result[row][col] = static_cast<float>(adjugate[row][col] * invDet);
    }
  }
}

void GE::Math::MtxInverse(dMat4& result, dMat4 const& mtx)
{
  double const det{ MtxDeterminant(mtx) };
  if (det == 0.0) { throw std::runtime_error("division by zero"); }

  // Calculate the adjugate of the input matrix
  dMat4 adjugate;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      // Calculate the cofactor for this element
      dMat3 submatrix;
      int sub_row = 0, sub_col = 0;
      for (int i = 0; i < 4; ++i) {
        if (i == row) continue;
        for (int j = 0; j < 4; ++j) {
          if (j == col) continue;
          submatrix[sub_row][sub_col] = mtx[i][j];
          sub_col++;
        }
        sub_row++;
      }
      double val1 = MtxDeterminant(submatrix);
      //double cofactor = ((row + col) % 2 == 0) ? MtxDeterminant(submatrix) : -MtxDeterminant(submatrix);
      double cofactor = ((row + col) % 2 == 0) ? val1 : -val1;

      // Fill in the adjugate matrix
      adjugate[row][col] = cofactor;
    }
  }

  // Calculate the inverse by dividing the adjugate by the determinant
  double invDet = 1.0 / det;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      result[row][col] = adjugate[row][col] * invDet;
    }
  }
}