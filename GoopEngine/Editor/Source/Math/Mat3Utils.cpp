/*!*********************************************************************
\file   Mat3Utils.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Matrix 3x3 utility functions (only for floats and doubles)


Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Mat3Utils.h"

using namespace GE::Math;

void MtxIdentity(Mat<3, 3, float>& mtx)
{
  mtx[0] = Mat<3, 3, float>::ValueType(1.f, 0.f, 0.f);
  mtx[1] = Mat<3, 3, float>::ValueType(0.f, 1.f, 0.f);
  mtx[2] = Mat<3, 3, float>::ValueType(0.f, 0.f, 1.f);
}

void MtxIdentity(Mat<3, 3, double>& mtx)
{
  mtx[0] = Mat<3, 3, double>::ValueType(1.0, 0.0, 0.0);
  mtx[1] = Mat<3, 3, double>::ValueType(0.0, 1.0, 0.0);
  mtx[2] = Mat<3, 3, double>::ValueType(0.0, 0.0, 1.0);
}

void MtxScale(Mat<3, 3, float>& mtx, float x, float y)
{
  mtx[0] = Mat<3, 3, float>::ValueType(x, 0.f, 0.f);
  mtx[1] = Mat<3, 3, float>::ValueType(0.f, y, 0.f);
  mtx[2] = Mat<3, 3, float>::ValueType(0.f, 0.f, 1.f);
}

void MtxScale(Mat<3, 3, double>& mtx, float x, float y)
{
  mtx[0] = Mat<3, 3, double>::ValueType(x, 0.0, 0.0);
  mtx[1] = Mat<3, 3, double>::ValueType(0.0, y, 0.0);
  mtx[2] = Mat<3, 3, double>::ValueType(0.0, 0.0, 1.0);
}

void MtxRotate(Mat<3, 3, float>& mtx, float radians)
{
  float const cosRad{ cosf(radians) }, sinRad{ sinf(radians) };
  mtx[0] = Mat<3, 3, float>::ValueType(cosRad, -sinRad, 0.f);
  mtx[1] = Mat<3, 3, float>::ValueType(sinRad, cosRad, 0.f);
  mtx[2] = Mat<3, 3, float>::ValueType(0.f, 0.f, 1.f);
}

void MtxRotate(Mat<3, 3, double>& mtx, float radians)
{
  double const cosRad{ cos(radians) }, sinRad{ sin(radians) };
  mtx[0] = Mat<3, 3, double>::ValueType(cosRad, -sinRad, 0.0);
  mtx[1] = Mat<3, 3, double>::ValueType(sinRad, cosRad, 0.0);
  mtx[2] = Mat<3, 3, double>::ValueType(0.0, 0.0, 1.0);
}

void MtxTranslate(Mat<3, 3, float>& mtx, float x, float y)
{
  mtx[0] = Mat<3, 3, float>::ValueType(0.f, 0.f, x);
  mtx[1] = Mat<3, 3, float>::ValueType(0.f, 0.f, y);
  mtx[2] = Mat<3, 3, float>::ValueType(0.f, 0.f, 1.f);
}

void MtxTranslate(Mat<3, 3, double>& mtx, float x, float y)
{
  mtx[0] = Mat<3, 3, double>::ValueType(0.0, 0.0, x);
  mtx[1] = Mat<3, 3, double>::ValueType(0.0, 0.0, y);
  mtx[2] = Mat<3, 3, double>::ValueType(0.0, 0.0, 1.0);
}

void MtxInverse(Mat<3, 3, float>& result, Mat<3, 3, float> const& mtx)
{
  float const det{ MtxDeterminant(mtx) };
  if (det == 0.f) { throw std::runtime_error("division by zero"); }

  float const inverseDet{ 1.f / det };
  Mat<3, 3, float> const temp{ mtx };
  result[0].x = (temp[1].y * temp[2].z - temp[1].z * temp[2].y) * inverseDet;
  result[1].x = (temp[2].x * temp[1].z - temp[1].x * temp[2].z) * inverseDet;
  result[2].x = (temp[1].x * temp[2].y - temp[2].x * temp[1].y) * inverseDet;
  result[0].y = (temp[2].y * temp[0].z - temp[0].y * temp[2].z) * inverseDet;
  result[1].y = (temp[0].x * temp[2].z - temp[2].x * temp[0].z) * inverseDet;
  result[2].y = (temp[0].y * temp[2].x - temp[0].x * temp[2].y) * inverseDet;
  result[0].z = (temp[0].y * temp[1].z - temp[0].z * temp[1].y) * inverseDet;
  result[1].z = (temp[0].z * temp[1].x - temp[0].x * temp[1].z) * inverseDet;
  result[2].z = (temp[0].x * temp[1].y - temp[0].y * temp[1].x) * inverseDet;
}

void MtxInverse(Mat<3, 3, double>& result, Mat<3, 3, double> const& mtx)
{
  double const det{ MtxDeterminant(mtx) };
  if (det == 0.0) { throw std::runtime_error("division by zero"); }

  double const inverseDet{ 1.0 / det };
  Mat<3, 3, double> const temp{ mtx };
  result[0].x = (temp[1].y * temp[2].z - temp[1].z * temp[2].y) * inverseDet;
  result[1].x = (temp[2].x * temp[1].z - temp[1].x * temp[2].z) * inverseDet;
  result[2].x = (temp[1].x * temp[2].y - temp[2].x * temp[1].y) * inverseDet;
  result[0].y = (temp[2].y * temp[0].z - temp[0].y * temp[2].z) * inverseDet;
  result[1].y = (temp[0].x * temp[2].z - temp[2].x * temp[0].z) * inverseDet;
  result[2].y = (temp[0].y * temp[2].x - temp[0].x * temp[2].y) * inverseDet;
  result[0].z = (temp[0].y * temp[1].z - temp[0].z * temp[1].y) * inverseDet;
  result[1].z = (temp[0].z * temp[1].x - temp[0].x * temp[1].z) * inverseDet;
  result[2].z = (temp[0].x * temp[1].y - temp[0].y * temp[1].x) * inverseDet;
}