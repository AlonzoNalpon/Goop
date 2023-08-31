// Constructors
template <typename T>
Vec<3, T>::Vec() : x{}, y{}, z{} {}

template <typename T>
Vec<3, T>::Vec(T _x, T _y, T _z) : x{_x}, y{_y}, z{_z} {}

template <typename T>
Vec<3, T>::Vec(Vec<3, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z} {}

template <typename T>
Vec<3, T>::Vec(Vec<2, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{} {}

template <typename T>
Vec<3, T>::Vec(Vec<4, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z} {}

// Operator overloads
template <typename T>
Vec<3, T>& Vec<3, T>::operator=(Vec<3, T> const& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator+=(Vec<3, T> const& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator+=(T rhs)
{
  x += rhs;
  y += rhs;
  z += rhs;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator-=(Vec<3, T> const& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator-=(T rhs)
{
  x -= rhs;
  y -= rhs;
  z -= rhs;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator*=(T rhs)
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}
template <typename T>
Vec<3, T>& Vec<3, T>::operator/=(T rhs)
{
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

template <typename T>
T& Vec<3, T>::operator[](unsigned rhs)
{
  return *(&x + rhs);
}
template <typename T>
T const& Vec<3, T>::operator[](unsigned rhs) const
{
  return *(&x + rhs);
}

// Member functions
template <typename T>
void Vec<3, T>::Normalize()
{
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");

  T inverse_root = 1 / sqrtf(x * x + y * y + z * z);
  x *= inverse_root;
  y *= inverse_root;
  z *= inverse_root;
}

// Non-member operator overloads
template <typename T>
Vec<3, T> operator+(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename T>
Vec<3, T> operator+(Vec<3, T> const& lhs, T rhs)
{
  return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs };
}

template <typename T>
Vec<3, T> operator+(T lhs, Vec<3, T> const& rhs)
{
  return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z };
}

template <typename T>
Vec<3, T> operator-(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template <typename T>
Vec<3, T> operator-(Vec<3, T> const& lhs, T rhs)
{
  return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs };
}

template <typename T>
Vec<3, T> operator-(Vec<3, T> const& rhs)
{
  return { -rhs.x, -rhs.y, -rhs.z };
}

template <typename T>
Vec<3, T> operator*(Vec<3, T> const& lhs, T rhs)
{
  return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

template <typename T>
Vec<3, T> operator*(T lhs, Vec<3, T> const& rhs)
{
  return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

template <typename T>
Vec<3, T> operator/(Vec<3, T> const& lhs, T rhs)
{
  return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

template <typename T>
bool operator==(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
bool operator!=(Vec<3, T> const& lhs, Vec<3, T> const& rhs)
{
  return !(lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
}
