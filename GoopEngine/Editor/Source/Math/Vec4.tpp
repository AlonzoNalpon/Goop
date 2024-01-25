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
Vec<4, T>::Vec(Vec<2, T> const& rhs, T _z, T _w) : x{rhs.x}, y{rhs.y}, z{_z}, w{_w} {}

template <typename T>
Vec<4, T>::Vec(Vec<3, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z}, w{} {}

template <typename T>
Vec<4, T>::Vec(Vec<3, T> const& rhs, T _w) : x{rhs.x}, y{rhs.y}, z{rhs.z}, w{_w} {}

template <typename T>
template <typename S>
Vec<4, T>::Vec(Vec<4, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ static_cast<T>(rhs.z) }, w{ static_cast<T>(rhs.w) } {}

template <typename T>
template <typename S>
Vec<4, T>::Vec(Vec<2, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{}, w{} {}

template <typename T>
template <typename S>
Vec<4, T>::Vec(Vec<2, S> const& rhs, T _z, T _w) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ _z }, w{ _w } {}

template <typename T>
template <typename S>
Vec<4, T>::Vec(Vec<3, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ static_cast<T>(rhs.z) }, w{} {}

template <typename T>
template <typename S>
Vec<4, T>::Vec(Vec<3, S> const& rhs, T _w) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ static_cast<T>(rhs.z) }, w{ _w } {}


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
Vec<4, T>& Vec<4, T>::operator-=(Vec<4, T> const& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
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
template <typename S>
Vec<4, T>& Vec<4, T>::operator+=(S rhs)
{
  x += static_cast<T>(rhs);
  y += static_cast<T>(rhs);
  z += static_cast<T>(rhs);
  w += static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<4, T>& Vec<4, T>::operator-=(S rhs)
{
  x -= static_cast<T>(rhs);
  y -= static_cast<T>(rhs);
  z -= static_cast<T>(rhs);
  w -= static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<4, T>& Vec<4, T>::operator*=(S rhs)
{
  x *= static_cast<T>(rhs);
  y *= static_cast<T>(rhs);
  z *= static_cast<T>(rhs);
  w *= static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<4, T>& Vec<4, T>::operator/=(S rhs)
{
  x /= static_cast<T>(rhs);
  y /= static_cast<T>(rhs);
  z /= static_cast<T>(rhs);
  w /= static_cast<T>(rhs);
  return *this;
}

template <typename T>
Vec<4, T>& Vec<4, T>::operator<<(std::string const& rhs)
{
  std::istringstream iss{ rhs.substr(1, rhs.size() - 3) };
  char ch1, ch2, ch3;
  iss >> x >> ch1 >> y >> ch2 >> z >> ch3 >> w;
  if (iss.fail() || ch1 != ',' || ch2 != ',' || ch3 != ',')
  {
	#ifdef _DEBUG
	//throw GE::Debug::Exception<Vec<2,T>>(GE::Debug::LEVEL_ERROR, ErrMsg("Invalid Vec2 string format"));
	#endif
	x = y = z = w = {};
  }

  return *this;
}

template <typename T>
Vec<4, T> Vec<4,T>::operator-() const
{
  return { -x, -y, -z, -w };
}

template <typename T>
T& Vec<4, T>::operator[](size_type rhs)
{
  return *(&x + rhs);
}
template <typename T>
T const& Vec<4, T>::operator[](size_type rhs) const
{
  return *(&x + rhs);
}

// Member functions
template <typename T>
void Vec<4, T>::Normalize()
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  #endif
  
  T inverse_root = 1 / sqrt(x * x + y * y + z * z + w * w);
  x *= inverse_root;
  y *= inverse_root;
  z *= inverse_root;
  w *= inverse_root;
}

template <typename T>
std::string Vec<4, T>::ToString() const
{
	std::ostringstream oss{};
	oss << "{ " << x << ", " << y  << ", " << z << ", " << w << " }";
	return oss.str();
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

template <typename T, typename S>
Vec<4, T> operator+(Vec<4, T> const& lhs, S rhs)
{
  return { lhs.x + static_cast<T>(rhs), lhs.y + static_cast<T>(rhs), lhs.z + static_cast<T>(rhs), lhs.w + static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<4, T> operator+(S lhs, Vec<4, T> const& rhs)
{
  return { static_cast<T>(lhs) + rhs.x, static_cast<T>(lhs) + rhs.y, static_cast<T>(lhs) + rhs.z, static_cast<T>(lhs) + rhs.w };
}

template <typename T, typename S>
Vec<4, T> operator-(Vec<4, T> const& lhs, S rhs)
{
  return { lhs.x - static_cast<T>(rhs), lhs.y - static_cast<T>(rhs), lhs.z - static_cast<T>(rhs), lhs.w - static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<4, T> operator*(Vec<4, T> const& lhs, S rhs)
{
  return { lhs.x * static_cast<T>(rhs), lhs.y * static_cast<T>(rhs), lhs.z * static_cast<T>(rhs), lhs.w * static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<4, T> operator*(S lhs, Vec<4, T> const& rhs)
{
  return { static_cast<T>(lhs) * rhs.x, static_cast<T>(lhs) * rhs.y, static_cast<T>(lhs) * rhs.z, static_cast<T>(lhs) * rhs.w };
}

template <typename T, typename S>
Vec<4, T> operator/(Vec<4, T> const& lhs, S rhs)
{
  return { lhs.x / static_cast<T>(rhs), lhs.y / static_cast<T>(rhs), lhs.z / static_cast<T>(rhs), lhs.w / static_cast<T>(rhs) };
}
