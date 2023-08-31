#ifndef GOOP_MAT_OPERATIONS_H
#define GOOP_MAT_OPERATIONS_H

namespace gpm
{

  template <unsigned N, unsigned M, unsigned P, typename T>
  Mat<N, P, T> operator*(Mat<N, M, T> const& lhs, Mat<M, P, T> const& rhs);

  void MtxIdentity(Mat3& mtx);
  void MtxScale(Mat3& mtx, float x, float y);
  void MtxRotate(Mat3& mtx, float radians);
  void MtxTranslate(Mat3& mtx, float x, float y);
  void MtxInverse(Mat3& result, Mat3 const& mtx);

  template <unsigned N, unsigned M, typename T>
  void MtxTranspose(Mat<N, M, T>& result, Mat<M, N, T> const& mtx);
  template <typename T>
  T MtxDeterminant(Mat<3, 3, T> const& mtx);
  
  #include "MatOperations.tpp"

} // namespace gpm

#endif