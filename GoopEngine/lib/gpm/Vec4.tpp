// Constructors
template <typename T>
Vec<4, T>::Vec() : x{}, y{}, z{}, w{} {}

template <typename T>
Vec<4, T>::Vec(T _x, T _y, T _z, T _w) : x{_x}, y{_y}, z{_z}, w{_w} {}

template <typename T>
Vec<4, T>::Vec(Vec<4, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z} , w{rhs.w} {}

template <typename T>
Vec<4, T>::Vec(Vec<2, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{}, w{} {}

template <typename T>
Vec<4, T>::Vec(Vec<3, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z}, w{} {}

// Operator overloads
template <typename T>
Vec<4, T>& Vec<4, T>::operator=(Vec<4, T> const& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator+=(Vec<4, T> const& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator+=(T rhs)
{
  x += rhs;
  y += rhs;
  z += rhs;
  w += rhs;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator-=(Vec<4, T> const& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator-=(T rhs)
{
  x -= rhs;
  y -= rhs;
  z -= rhs;
  w -= rhs;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator*=(T rhs)
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  w *= rhs;
  return *this;
}
template <typename T>
Vec<4, T>& Vec<4, T>::operator/=(T rhs)
{
  x /= rhs;
  y /= rhs;
  z /= rhs;
  w /= rhs;
  return *this;
}

template <typename T>
T& Vec<4, T>::operator[](unsigned rhs)
{
  return *(&x + rhs);
}
template <typename T>
T const& Vec<4, T>::operator[](unsigned rhs) const
{
  return *(&x + rhs);
}

// Member functions
template <typename T>
void Vec<4, T>::Normalize()
{
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  
  T inverse_root = 1 / sqrtf(x * x + y * y + z * z + w * w);
  x *= inverse_root;
  y *= inverse_root;
  z *= inverse_root;
  w *= inverse_root;
}

// Non-member operator overloads
template <typename T>
Vec<4, T> operator+(Vec<4, T> const& lhs, Vec<4, T> const& rhs)
{
  return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

template <typename T>
Vec<4, T> operator+(Vec<4, T> const& lhs, T rhs)
{
  return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs };
}

template <typename T>
Vec<4, T> operator+(T lhs, Vec<4, T> const& rhs)
{
  return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w };
}

template <typename T>
Vec<4, T> operator-(Vec<4, T> const& lhs, Vec<4, T> const& rhs)
{
  return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

template <typename T>
Vec<4, T> operator-(Vec<4, T> const& lhs, T rhs)
{
  return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs };
}

template <typename T>
Vec<4, T> operator-(Vec<4, T> const& rhs)
{
  return { -rhs.x, -rhs.y, -rhs.z, -rhs.w };
}

template <typename T>
Vec<4, T> operator*(Vec<4, T> const& lhs, T rhs)
{
  return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

template <typename T>
Vec<4, T> operator*(T lhs, Vec<4, T> const& rhs)
{
  return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w };
}

template <typename T>
Vec<4, T> operator/(Vec<4, T> const& lhs, T rhs)
{
  return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

template <typename T>
bool operator==(Vec<4, T> const& lhs, Vec<4, T> const& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename T>
bool operator!=(Vec<4, T> const& lhs, Vec<4, T> const& rhs)
{
  return !(lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
}