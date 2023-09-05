// Constructors
template <typename T>
Mat<3, 3, T>::Mat()
{
  for (size_type i{}; i < 3; ++i)
  {
    m_data[i] = ValueType();
  }
}

template <typename T>
Mat<3, 3, T>::Mat(T m00, T m01, T m02, T m10, 
                  T m11, T m12, T m20, T m21, T m22)
{
  m_data[0] = { m00, m01, m02 };
  m_data[1] = { m10, m11, m12 };
  m_data[2] = { m20, m21, m22 };
}

template <typename T>
Mat<3, 3, T>::Mat(Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    m_data[i] = rhs[i];
  }
}

template <typename T>
Mat<3, 3, T>::Mat(ValueType const& row0, ValueType const& row1,  ValueType const& row2) 
{
  m_data[0] = row0;
  m_data[1] = row1;
  m_data[2] = row2;
}

// Operator overloads
template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator=(Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    m_data[i] = rhs[i];
  }

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator+=(T rhs)
{
  m_data[0] += rhs;
  m_data[1] += rhs;
  m_data[2] += rhs;

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator+=(Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    m_data[i] += rhs[i];
  }

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator-=(T rhs)
{
  m_data[0] -= rhs;
  m_data[1] -= rhs;
  m_data[2] -= rhs;

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator-=(Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    m_data[i] -= rhs[i];
  }

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator*=(T rhs)
{
  m_data[0] *= rhs;
  m_data[1] *= rhs;
  m_data[2] *= rhs;

  return *this;
}

template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator/=(T rhs)
{
  m_data[0] /= rhs;
  m_data[1] /= rhs;
  m_data[2] /= rhs;

  return *this;
}

template <typename T>
Mat<3, 3, T> Mat<3, 3, T>::operator-() const
{
  return { -m_data[0], -m_data[1], -m_data[2] };
}

// Accessors
template <typename T>
typename Mat<3, 3, T>::ValueType& Mat<3, 3, T>::operator[](size_type rhs)
{
  return *(m_data + rhs);
}

template <typename T>
typename Mat<3, 3, T>::ValueType const& Mat<3, 3, T>::operator[](size_type rhs) const
{
  return *(m_data + rhs);
}

template <typename T>
T& Mat<3, 3, T>::At(size_type col, size_type row)
{
  if ((col > 2 || col < 0) || (row > 2 || row < 0))
  {
    throw std::out_of_range("At: out of range");
  }

  return m_data[row][col];
}

template <typename T>
T const& Mat<3, 3, T>::At(size_type col, size_type row) const
{
  if ((col > 2 || col < 0) || (row > 2 || row < 0))
  {
    throw std::out_of_range("At (const): out of range");
  }

  return m_data[row][col];
}

// Member Functions
template <typename T>
typename Mat<3, 3, T>::ValueType Mat<3, 3, T>::GetCol(size_type col) const
{
  if (col > 2)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return { m_data[0][col], m_data[1][col], m_data[2][col] };
}

template <typename T>
typename Mat<3, 3, T>::ValueType Mat<3, 3, T>::GetRow(size_type row) const
{
  if (row > 2)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return m_data[row];
}

template <typename T>
void Mat<3, 3, T>::Transpose()
{
  std::swap(m_data[0].y, m_data[1].x);
  std::swap(m_data[0].z, m_data[2].x);
  std::swap(m_data[1].z, m_data[2].y);
}

// Non-member operator overloads
template <typename T>
Mat<3, 3, T> operator+(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] };
}

template <typename T>
Mat<3, 3, T> operator+(Mat<3, 3, T> const& lhs, T rhs)
{
  return { lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs };
}

template <typename T>
Mat<3, 3, T> operator-(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] };
}

template <typename T>
Mat<3, 3, T> operator-(Mat<3, 3, T> const& lhs, T rhs)
{
  return { lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs };
}

template <typename T>
Mat<3, 3, T> operator*(T lhs, Mat<3, 3, T> const& rhs)
{
  return { lhs * rhs[0], lhs * rhs[1], lhs * rhs[2] };
}

template <typename T>
Mat<3, 3, T> operator*(Mat<3, 3, T> const& lhs, T rhs)
{
  return { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs }; 
}

template <typename T>
Vec<3, T> operator*(Mat<3, 3, T> const& lhs, Vec<3, T> const& rhs)
{
  return { Dot(lhs[0], rhs), Dot(lhs[1], rhs), Dot(lhs[2], rhs) };
}

template <typename T>
Mat<3, 3, T> operator/(Mat<3, 3, T> const& lhs, T rhs)
{
  return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs };
}

template <typename T>
bool operator==(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <typename T>
bool operator!=(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  for (size_type i{}; i < 3; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}