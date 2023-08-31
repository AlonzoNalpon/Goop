#ifndef GOOP_VEC_UTILS_H
#define GOOP_VEC_UTILS_H

namespace gpm
{
  template<typename T>
  Vec<2, T> Normalize(Vec<2, T> const& lhs);
  template<typename T>
  Vec<3, T> Normalize(Vec<3, T> const& lhs);
  template<typename T>
  Vec<4, T> Normalize(Vec<4, T> const& lhs);

  template<typename T>
  T GetMagnitude(Vec<2, T> const& lhs);
  template<typename T>
  T GetMagnitude(Vec<3, T> const& lhs);
  template<typename T>
  T GetMagnitude(Vec<4, T> const& lhs);
  template<typename T>
  T GetSquaredMagnitude(Vec<2, T> const& lhs);
  template<typename T>
  T GetSquaredMagnitude(Vec<3, T> const& lhs);
  template<typename T>
  T GetSquaredMagnitude(Vec<4, T> const& lhs);

  template<typename T>
  T Dot(Vec<2, T> const& lhs, Vec<2, T> const& rhs);
  template<typename T>
  T Dot(Vec<3, T> const& lhs, Vec<3, T> const& rhs);
  template<typename T>
  T Dot(Vec<4, T> const& lhs, Vec<4, T> const& rhs);

  template<typename T>
  Vec<3, T> Cross(Vec<3, T> const& lhs, Vec<3, T> const& rhs);

  #include "VecUtils.tpp"

} // namespace gpm

#endif