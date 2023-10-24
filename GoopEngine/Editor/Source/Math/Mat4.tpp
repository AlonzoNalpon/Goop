// Constructors
template <typename T>
Mat<4, 4, T>::Mat()
{
  m_data[0] = m_data[1] = m_data[2] = m_data[3] = ValueType();
}

template <typename T>
Mat<4, 4, T>::Mat(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13,
				T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
{
  m_data[0].x = m00; m_data[0].y = m01; m_data[0].z = m02; m_data[0].w = m03;
  m_data[1].x = m10; m_data[1].y = m11; m_data[1].z = m12; m_data[1].w = m13;
  m_data[2].x = m20; m_data[2].y = m21; m_data[2].z = m22; m_data[2].w = m23;
  m_data[3].x = m30; m_data[3].y = m31; m_data[3].z = m32; m_data[3].w = m33;
}

template <typename T>
Mat<4, 4, T>::Mat(Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] = rhs[i];
  }
}

template <typename T>
Mat<4, 4, T>::Mat(Mat<4, 4, T>&& rhs)
{
  std::swap(m_data, rhs.m_data);
}

template <typename T>
Mat<4, 4, T>::Mat(ValueType const& row0, ValueType const& row1,
                  ValueType const& row2, ValueType const& row3) 
{
  m_data[0] = row0;
  m_data[1] = row1;
  m_data[2] = row2;
  m_data[3] = row3;
}

template <typename T>
Mat<4, 4, T>::Mat(ValueType&& row0, ValueType&& row1, ValueType&& row2, ValueType&& row3)
{
  m_data[0] = std::move(row0);
  m_data[1] = std::move(row1);
  m_data[2] = std::move(row2);
  m_data[3] = std::move(row3);
}

// Operator overloads
template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator=(Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] = rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator+=(T rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] += rhs;
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator+=(Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] += rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator-=(T rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] -= rhs;
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator-=(Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] -= rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator*=(T rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] *= rhs;
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator/=(T rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    m_data[i] /= rhs;
  }

  return *this;
}

template <typename T>
Mat<4, 4, T> Mat<4, 4, T>::operator-() const
{
  return { -m_data[0], -m_data[1], -m_data[2], -m_data[3] };
}

// Accessors
template <typename T>
typename Mat<4, 4, T>::ValueType& Mat<4, 4, T>::operator[](size_type rhs)
{
  return *(m_data + rhs);
}

template <typename T>
typename Mat<4, 4, T>::ValueType const& Mat<4, 4, T>::operator[](size_type rhs) const
{
  return *(m_data + rhs);
}

template <typename T>
T& Mat<4, 4, T>::At(size_type col, size_type row)
{
  if ((col > 3 || col < 0) || (row > 3 || row < 0))
  {
    throw std::out_of_range("At: out of range");
  }

  return m_data[row][col];
}

template <typename T>
T const& Mat<4, 4, T>::At(size_type col, size_type row) const
{
  if ((col > 3 || col < 0) || (row > 3 || row < 0))
  {
    throw std::out_of_range("At (const): out of range");
  }

  return m_data[row][col];
}

// Member Functions
template <typename T>
typename Mat<4, 4, T>::ValueType Mat<4, 4, T>::GetCol(size_type col) const
{
  if (col > 3)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return { m_data[0][col], m_data[1][col], m_data[2][col], m_data[3][col] };
}

template <typename T>
typename Mat<4, 4, T>::ValueType Mat<4, 4, T>::GetRow(size_type row) const
{
  if (row > 3)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return m_data[row];
}

template <typename T>
void Mat<4, 4, T>::Transpose()
{
  Mat<4, 4, T> temp{ *this };
  temp[0] = GetCol(0);
  temp[1] = GetCol(1);
  temp[2] = GetCol(2);
  temp[3] = GetCol(3);

  *this = temp;
}

// Non-member operator overloads
template <typename T>
Mat<4, 4, T> operator+(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] };
}

template <typename T>
Mat<4, 4, T> operator+(Mat<4, 4, T> const& lhs, T rhs)
{
  return { lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs , lhs[3] + rhs };
}

template <typename T>
Mat<4, 4, T> operator-(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] };
}

template <typename T>
Mat<4, 4, T> operator-(Mat<4, 4, T> const& lhs, T rhs)
{
  return { lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs, lhs[3] - rhs };
}

template <typename T>
Mat<4, 4, T> operator*(T lhs, Mat<4, 4, T> const& rhs)
{
  return { lhs * rhs[0], lhs * rhs[1], lhs * rhs[2], lhs * rhs[3] };
}

template <typename T>
Mat<4, 4, T> operator*(Mat<4, 4, T> const& lhs, T rhs)
{
  return { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs }; 
}

template <typename T>
Vec<4, T> operator*(Mat<4, 4, T> const& lhs, Vec<4, T> const& rhs)
{
  return { Dot(lhs[0], rhs), Dot(lhs[1], rhs), 
           Dot(lhs[2], rhs), Dot(lhs[3], rhs) };
}

template <typename T>
Mat<4, 4, T> operator/(Mat<4, 4, T> const& lhs, T rhs)
{
  return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs, lhs[3] / rhs };
}

template <typename T>
bool operator==(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <typename T>
bool operator!=(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  for (size_type i{}; i < 4; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}