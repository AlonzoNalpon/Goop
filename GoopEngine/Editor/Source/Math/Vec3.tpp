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
Vec<3, T>::Vec(Vec<2, T> const& rhs, T _z) : x{rhs.x}, y{rhs.y}, z{_z} {}

template <typename T>
Vec<3, T>::Vec(Vec<4, T> const& rhs) : x{rhs.x}, y{rhs.y}, z{rhs.z} {}

template <typename T>
template <typename S>
Vec<3, T>::Vec(Vec<3, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ static_cast<T>(rhs.z) } {}

template <typename T>
template <typename S>
Vec<3, T>::Vec(Vec<2, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{} {}

template <typename T>
template <typename S>
Vec<3, T>::Vec(Vec<4, S> const& rhs) : x{ static_cast<T>(rhs.x) }, y{ static_cast<T>(rhs.y) }, z{ static_cast<T>(rhs.z) } {}


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
Vec<3, T>& Vec<3, T>::operator-=(Vec<3, T> const& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
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
template <typename S>
Vec<3, T>& Vec<3, T>::operator+=(S rhs)
{
  x += static_cast<T>(rhs);
  y += static_cast<T>(rhs);
  z += static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<3, T>& Vec<3, T>::operator-=(S rhs)
{
  x -= static_cast<T>(rhs);
  y -= static_cast<T>(rhs);
  z -= static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<3, T>& Vec<3, T>::operator*=(S rhs)
{
  x *= static_cast<T>(rhs);
  y *= static_cast<T>(rhs);
  z *= static_cast<T>(rhs);
  return *this;
}

template <typename T>
template <typename S>
Vec<3, T>& Vec<3, T>::operator/=(S rhs)
{
  x /= static_cast<T>(rhs);
  y /= static_cast<T>(rhs);
  z /= static_cast<T>(rhs);
  return *this;
}

template <typename T>
Vec<3, T>& Vec<3, T>::operator<<(std::string const& rhs)
{
  std::istringstream iss{ rhs.substr(1, rhs.size() - 3) };
  char ch1, ch2;
  iss >> x >> ch1 >> y >> ch2 >> z;
  if (iss.fail() || ch1 != ',' || ch2 != ',')
  {
	#ifdef _DEBUG
	//throw GE::Debug::Exception<Vec<2,T>>(GE::Debug::LEVEL_ERROR, ErrMsg("Invalid Vec2 string format"));
	#endif
	x = y = z = {};
  }

  return *this;
}

template <typename T>
Vec<3, T> Vec<3,T>::operator-() const
{
  return { -x, -y, -z };
}

template <typename T>
T& Vec<3, T>::operator[](size_type rhs)
{
  return *(&x + rhs);
}
template <typename T>
T const& Vec<3, T>::operator[](size_type rhs) const
{
  return *(&x + rhs);
}


// Member functions
template <typename T>
void Vec<3, T>::Normalize()
{
  #ifdef _DEBUG
  static_assert(std::is_floating_point<T>::value, "Normalize can only be called on floating point-typed Vectors");
  #endif

  T inverse_root = 1 / sqrt(x * x + y * y + z * z);
  x *= inverse_root;
  y *= inverse_root;
  z *= inverse_root;
}

template <typename T>
std::string Vec<3, T>::ToString() const
{
	std::ostringstream oss{};
	oss << "{ " << x << ", " << y  << ", " << z << " }";
	return oss.str();
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

template <typename T, typename S>
Vec<3, T> operator+(Vec<3, T> const& lhs, S rhs)
{
  return { lhs.x + static_cast<T>(rhs), lhs.y + static_cast<T>(rhs), lhs.z + static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<3, T> operator+(S lhs, Vec<3, T> const& rhs)
{
  return { static_cast<T>(lhs) + rhs.x, static_cast<T>(lhs) + rhs.y, static_cast<T>(lhs) + rhs.z };
}

template <typename T, typename S>
Vec<3, T> operator-(Vec<3, T> const& lhs, S rhs)
{
  return { lhs.x - static_cast<T>(rhs), lhs.y - static_cast<T>(rhs), lhs.z - static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<3, T> operator*(Vec<3, T> const& lhs, S rhs)
{
  return { lhs.x * static_cast<T>(rhs), lhs.y * static_cast<T>(rhs), lhs.z * static_cast<T>(rhs) };
}

template <typename T, typename S>
Vec<3, T> operator*(S lhs, Vec<3, T> const& rhs)
{
  return { static_cast<T>(lhs) * rhs.x, static_cast<T>(lhs) * rhs.y, static_cast<T>(lhs) * rhs.z };
}

template <typename T, typename S>
Vec<3, T> operator/(Vec<3, T> const& lhs, S rhs)
{
  return { lhs.x / static_cast<T>(rhs), lhs.y / static_cast<T>(rhs), lhs.z / static_cast<T>(rhs) };
}
