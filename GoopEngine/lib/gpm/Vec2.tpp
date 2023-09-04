// Constructors
template <typename T>
Vec<2, T>::Vec() : x{}, y{} {}

template <typename T>
Vec<2, T>::Vec(T _x, T _y) : x{_x}, y{_y} {}

template <typename T>
Vec<2, T>::Vec(Vec<2, T> const& rhs) : x{rhs.x}, y{rhs.y} {}

template <typename T>
Vec<2, T>::Vec(Vec<3, T> const& rhs) : x{rhs.x}, y{rhs.y} {}

template <typename T>
Vec<2, T>::Vec(Vec<4, T> const& rhs) : x{rhs.x}, y{rhs.y} {}

// Operator overloads
template <typename T>
Vec<2, T>& Vec<2, T>::operator=(Vec<2, T> const& rhs)
{
  x = rhs.x;
  y = rhs.y;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator+=(Vec<2, T> const& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator+=(T rhs)
{
  x += rhs;
  y += rhs;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator-=(Vec<2, T> const& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator-=(T rhs)
{
  x -= rhs;
  y -= rhs;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator*=(T rhs)
{
  x *= rhs;
  y *= rhs;
  return *this;
}
template <typename T>
Vec<2, T>& Vec<2, T>::operator/=(T rhs)
{
  x /= rhs;
  y /= rhs;
  return *this;
}

template <typename T>
Vec<2, T> Vec<2,T>::operator-() const
{
  return { -x, -y };
}

template <typename T>
T& Vec<2, T>::operator[](size_type rhs)
{
  return *(&x + rhs);
}
template <typename T>
T const& Vec<2, T>::operator[](size_type rhs) const
{
  return *(&x + rhs);
}

// Member functions
template <typename T>
void Vec<2, T>::Normalize()
{
  static_assert(std::is_floating_point<T>::value, "normalize can only be called on floating point-typed Vectors");

  T inverse_root = 1 / sqrtf(x * x + y * y);
  x *= inverse_root;
  y *= inverse_root;
}

// Non-member operator overloads
template <typename T>
Vec<2, T> operator+(Vec<2, T> const& lhs, Vec<2, T> const& rhs) \
{
  return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template <typename T>
Vec<2, T> operator+(Vec<2, T> const& lhs, T rhs) \
{
  return { lhs.x + rhs, lhs.y + rhs };
}

template <typename T>
Vec<2, T> operator+(T lhs, Vec<2, T> const& rhs)
{
  return { lhs + rhs.x, lhs + rhs.y };
}

template <typename T>
Vec<2, T> operator-(Vec<2, T> const& lhs, Vec<2, T> const& rhs)
{
  return { lhs.x - rhs.x, lhs.y - rhs.y };
}

template <typename T>
Vec<2, T> operator-(Vec<2, T> const& lhs, T rhs)
{
  return { lhs.x - rhs, lhs.y - rhs };
}

template <typename T>
Vec<2, T> operator*(Vec<2, T> const& lhs, T rhs)
{
  return { lhs.x * rhs, lhs.y * rhs };
}

template <typename T>
Vec<2, T> operator*(T lhs, Vec<2, T> const& rhs)
{
  return { lhs * rhs.x, lhs * rhs.y };
}

template <typename T>
Vec<2, T> operator/(Vec<2, T> const& lhs, T rhs)
{
  return { lhs.x / rhs, lhs.y / rhs };
}

template <typename T>
bool operator==(Vec<2, T> const& lhs, Vec<2, T> const& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
bool operator!=(Vec<2, T> const& lhs, Vec<2, T> const& rhs)
{
  return !(lhs.x == rhs.x && lhs.y == rhs.y);
}
