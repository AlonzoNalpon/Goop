/*!*********************************************************************
\file   Mat4Utils.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Matrix 4x4 utility functions (only for floats and doubles)


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>

using namespace GE::Math;


void GE::Math::MtxInverse(Mat<4, 4, float>& result, Mat<4, 4, float> const& mtx)
{
  Mat<4, 4, float> const temp{ mtx };

  float const A2323 = temp[2][2] * temp[3][3] - temp[2][3] * temp[3][2];
  float const A1323 = temp[2][1] * temp[3][3] - temp[2][3] * temp[3][1];
  float const A1223 = temp[2][1] * temp[3][2] - temp[2][2] * temp[3][1];
  float const A0323 = temp[2][0] * temp[3][3] - temp[2][3] * temp[3][0];
  float const A0223 = temp[2][0] * temp[3][2] - temp[2][2] * temp[3][0];
  float const A0123 = temp[2][0] * temp[3][1] - temp[2][1] * temp[3][0];
  float const A2313 = temp[1][2] * temp[3][3] - temp[1][3] * temp[3][2];
  float const A1313 = temp[1][1] * temp[3][3] - temp[1][3] * temp[3][1];
  float const A1213 = temp[1][1] * temp[3][2] - temp[1][2] * temp[3][1];
  float const A2312 = temp[1][2] * temp[2][3] - temp[1][3] * temp[2][2];
  float const A1312 = temp[1][1] * temp[2][3] - temp[1][3] * temp[2][1];
  float const A1212 = temp[1][1] * temp[2][2] - temp[1][2] * temp[2][1];
  float const A0313 = temp[1][0] * temp[3][3] - temp[1][3] * temp[3][0];
  float const A0213 = temp[1][0] * temp[3][2] - temp[1][2] * temp[3][0];
  float const A0312 = temp[1][0] * temp[2][3] - temp[1][3] * temp[2][0];
  float const A0212 = temp[1][0] * temp[2][2] - temp[1][2] * temp[2][0];
  float const A0113 = temp[1][0] * temp[3][1] - temp[1][1] * temp[3][0];
  float const A0112 = temp[1][0] * temp[2][1] - temp[1][1] * temp[2][0];

  float const det{ temp[0].x * (temp[1].y * A2323 - temp[1].z * A1323 + temp[1].w * A1223)
    - temp[0].y * (temp[1].x * A2323 - temp[1].z * A0323 + temp[1].w * A0223)
    + temp[0].z * (temp[1].x * A1323 - temp[1].y * A0323 + temp[1].w * A0123)
    - temp[0].w * (temp[1].x * A1223 - temp[1].y * A0223 + temp[1].z * A0123) };

  if (det == 0.f) { throw std::runtime_error("division by zero"); }
  float const invDet{ 1.f / det };

  result[0].x = invDet * (temp[1].y * A2323 - temp[1].z * A1323 + temp[1].w * A1223);
  result[0].y = invDet * -(temp[0].y * A2323 - temp[0].z * A1323 + temp[0].w * A1223);
  result[0].z = invDet * (temp[0].y * A2313 - temp[0].z * A1313 + temp[0].w * A1213);
  result[0].w = invDet * -(temp[0].y * A2312 - temp[0].z * A1312 + temp[0].w * A1212);
  result[1].x = invDet * -(temp[1].x * A2323 - temp[1].z * A0323 + temp[1].w * A0223);
  result[1].y = invDet * (temp[0].x * A2323 - temp[0].z * A0323 + temp[0].w * A0223);
  result[1].z = invDet * -(temp[0].x * A2313 - temp[0].z * A0313 + temp[0].w * A0213);
  result[1].w = invDet * (temp[0].x * A2312 - temp[0].z * A0312 + temp[0].w * A0212);
  result[2].x = invDet * (temp[1].x * A1323 - temp[1].y * A0323 + temp[1].w * A0123);
  result[2].y = invDet * -(temp[0].x * A1323 - temp[0].y * A0323 + temp[0].w * A0123);
  result[2].z = invDet * (temp[0].x * A1313 - temp[0].y * A0313 + temp[0].w * A0113);
  result[2].w = invDet * -(temp[0].x * A1312 - temp[0].y * A0312 + temp[0].w * A0112);
  result[3].x = invDet * -(temp[1].x * A1223 - temp[1].y * A0223 + temp[1].z * A0123);
  result[3].y = invDet * (temp[0].x * A1223 - temp[0].y * A0223 + temp[0].z * A0123);
  result[3].z = invDet * -(temp[0].x * A1213 - temp[0].y * A0213 + temp[0].z * A0113);
  result[3].w = invDet * (temp[0].x * A1212 - temp[0].y * A0212 + temp[0].z * A0112);
}

void GE::Math::MtxInverse(Mat<4, 4, double>& result, Mat<4, 4, double> const& mtx)
{
  Mat<4, 4, double> const temp{ mtx };

  double const A2323 = temp[2][2] * temp[3][3] - temp[2][3] * temp[3][2];
  double const A1323 = temp[2][1] * temp[3][3] - temp[2][3] * temp[3][1];
  double const A1223 = temp[2][1] * temp[3][2] - temp[2][2] * temp[3][1];
  double const A0323 = temp[2][0] * temp[3][3] - temp[2][3] * temp[3][0];
  double const A0223 = temp[2][0] * temp[3][2] - temp[2][2] * temp[3][0];
  double const A0123 = temp[2][0] * temp[3][1] - temp[2][1] * temp[3][0];
  double const A2313 = temp[1][2] * temp[3][3] - temp[1][3] * temp[3][2];
  double const A1313 = temp[1][1] * temp[3][3] - temp[1][3] * temp[3][1];
  double const A1213 = temp[1][1] * temp[3][2] - temp[1][2] * temp[3][1];
  double const A2312 = temp[1][2] * temp[2][3] - temp[1][3] * temp[2][2];
  double const A1312 = temp[1][1] * temp[2][3] - temp[1][3] * temp[2][1];
  double const A1212 = temp[1][1] * temp[2][2] - temp[1][2] * temp[2][1];
  double const A0313 = temp[1][0] * temp[3][3] - temp[1][3] * temp[3][0];
  double const A0213 = temp[1][0] * temp[3][2] - temp[1][2] * temp[3][0];
  double const A0312 = temp[1][0] * temp[2][3] - temp[1][3] * temp[2][0];
  double const A0212 = temp[1][0] * temp[2][2] - temp[1][2] * temp[2][0];
  double const A0113 = temp[1][0] * temp[3][1] - temp[1][1] * temp[3][0];
  double const A0112 = temp[1][0] * temp[2][1] - temp[1][1] * temp[2][0];

  double const det{ temp[0].x * (temp[1].y * A2323 - temp[1].z * A1323 + temp[1].w * A1223)
    - temp[0].y * (temp[1].x * A2323 - temp[1].z * A0323 + temp[1].w * A0223)
    + temp[0].z * (temp[1].x * A1323 - temp[1].y * A0323 + temp[1].w * A0123)
    - temp[0].w * (temp[1].x * A1223 - temp[1].y * A0223 + temp[1].z * A0123) };

  if (det <= 0.0) { throw GE::Debug::Exception<dMat4>(GE::Debug::LEVEL_CRITICAL, ErrMsg("Matrix determinant is 0!")); }
  double const invDet{ 1.0 / det };

  result[0].x = invDet * (temp[1].y * A2323 - temp[1].z * A1323 + temp[1].w * A1223);
  result[0].y = invDet * -(temp[0].y * A2323 - temp[0].z * A1323 + temp[0].w * A1223);
  result[0].z = invDet * (temp[0].y * A2313 - temp[0].z * A1313 + temp[0].w * A1213);
  result[0].w = invDet * -(temp[0].y * A2312 - temp[0].z * A1312 + temp[0].w * A1212);
  result[1].x = invDet * -(temp[1].x * A2323 - temp[1].z * A0323 + temp[1].w * A0223);
  result[1].y = invDet * (temp[0].x * A2323 - temp[0].z * A0323 + temp[0].w * A0223);
  result[1].z = invDet * -(temp[0].x * A2313 - temp[0].z * A0313 + temp[0].w * A0213);
  result[1].w = invDet * (temp[0].x * A2312 - temp[0].z * A0312 + temp[0].w * A0212);
  result[2].x = invDet * (temp[1].x * A1323 - temp[1].y * A0323 + temp[1].w * A0123);
  result[2].y = invDet * -(temp[0].x * A1323 - temp[0].y * A0323 + temp[0].w * A0123);
  result[2].z = invDet * (temp[0].x * A1313 - temp[0].y * A0313 + temp[0].w * A0113);
  result[2].w = invDet * -(temp[0].x * A1312 - temp[0].y * A0312 + temp[0].w * A0112);
  result[3].x = invDet * -(temp[1].x * A1223 - temp[1].y * A0223 + temp[1].z * A0123);
  result[3].y = invDet * (temp[0].x * A1223 - temp[0].y * A0223 + temp[0].z * A0123);
  result[3].z = invDet * -(temp[0].x * A1213 - temp[0].y * A0213 + temp[0].z * A0113);
  result[3].w = invDet * (temp[0].x * A1212 - temp[0].y * A0212 + temp[0].z * A0112);
}
