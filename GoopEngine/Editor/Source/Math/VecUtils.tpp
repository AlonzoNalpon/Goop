template<typename T>
Vec<2, T> Normalize(Vec<2, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  #endif

  T inverse_root = 1 / sqrt(vect.x * vect.x + vect.y * vect.y);
  return vect * inverse_root;
}

template<typename T>
Vec<3, T> Normalize(Vec<3, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  #endif

  T inverse_root = 1 / sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
  return vect * inverse_root;
}

template<typename T>
Vec<4, T> Normalize(Vec<4, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  #endif

  T inverse_root = 1 / sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z + vect.w * vect.w);
  return vect * inverse_root;
}

template<typename T>
T GetMagnitude(Vec<2, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");
  #endif

  return sqrt(vect.x * vect.x + vect.y * vect.y);
}

template<typename T>
T GetMagnitude(Vec<3, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");
  #endif

  return sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
}

template<typename T>
T GetMagnitude(Vec<4, T> const& vect)
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "GetMagnitude can only be called on floating point-typed Vectors");
  #endif

  return sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z + vect.w * vect.w);
}
