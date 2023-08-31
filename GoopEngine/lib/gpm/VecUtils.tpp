template<typename T>
Vec<2, T> Normalize(Vec<2, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "normalize can only be called on floating point-typed Vectors");

  T inverse_root = 1 / sqrtf(lhs.x * lhs.x + lhs.y * lhs.y);
  return lhs * inverse_root;
}

template<typename T>
Vec<3, T> Normalize(Vec<3, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "normalize can only be called on floating point-typed Vectors");

  T inverse_root = 1 / sqrtf(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z);
  return lhs * inverse_root;
}

template<typename T>
Vec<4, T> Normalize(Vec<4, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "normalize can only be called on floating point-typed Vectors");

  T inverse_root = 1 / sqrtf(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w);
  return lhs * inverse_root;
}

template<typename T>
T GetMagnitude(Vec<2, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");

  return sqrtf(lhs.x * lhs.x + lhs.y * lhs.y);
}

template<typename T>
T GetMagnitude(Vec<3, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");
    
  return sqrtf(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z);
}

template<typename T>
T GetMagnitude(Vec<4, T> const& lhs)
{
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");
    
  return sqrtf(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w);
}

template<typename T>
T GetSquaredMagnitude(Vec<2, T> const& lhs)
{
  return lhs.x * lhs.x + lhs.y * lhs.y;
}

template<typename T>
T GetSquaredMagnitude(Vec<3, T> const& lhs)
{
  return lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z;
}

template<typename T>
T GetSquaredMagnitude(Vec<4, T> const& lhs)
{
  return lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w;
}

template<typename T>
T Dot(Vec<2, T> const& lhs, Vec<2, T> const& rhs)
{
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T>
T Dot(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
T Dot(Vec<4, T> const& lhs, Vec<4, T> const& rhs)
{
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template<typename T>
Vec<3, T> Cross(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return
  { lhs.y * rhs.z - rhs.y * lhs.z,
    rhs.x * lhs.z - lhs.x * rhs.z,
    lhs.x * rhs.y - rhs.x * lhs.y };
}
