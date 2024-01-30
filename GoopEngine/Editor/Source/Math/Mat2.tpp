// Constructors
template <typename T>
Mat<2, 2, T>::Mat()
{
  m_data[0] = m_data[1] = ValueType();
}

template <typename T>
Mat<2, 2, T>::Mat(T m00, T m01, T m10, T m11)
{
  m_data[0] = ValueType(m00, m01);
  m_data[1] = ValueType(m10, m11);
}

template <typename T>
Mat<2, 2, T>::Mat(Mat<2, 2, T>&& rhs)
{
  m_data[0] = std::move(rhs.m_data[0]);
  m_data[1] = std::move(rhs.m_data[1]);
}

template <typename T>
Mat<2, 2, T>::Mat(Mat<2, 2, T> const& rhs)
{
  m_data[0] = rhs.m_data[0];
  m_data[1] = rhs.m_data[1];
}

template <typename T>
Mat<2, 2, T>::Mat(ValueType&& row0, ValueType&& row1) 
{
  m_data[0] = std::move(row0);
  m_data[1] = std::move(row1);
}

template <typename T>
Mat<2, 2, T>::Mat(ValueType const& row0, ValueType const& row1) 
{
  m_data[0] = row0;
  m_data[1] = row1;
}

template <typename T>
template <typename S>
Mat<2, 2, T>::Mat(Mat<2, 2, S> const& rhs)
{
  m_data[0] = rhs.m_data[0];
  m_data[1] = rhs.m_data[1];
}


// Operator overloads
template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator=(Mat<2, 2, T>&& rhs)
{
  m_data[0] = std::move(rhs.m_data[0]);
  m_data[1] = std::move(rhs.m_data[1]);

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator=(Mat<2, 2, T> const& rhs)
{
  m_data[0] = rhs.m_data[0];
  m_data[1] = rhs.m_data[1];

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator+=(T rhs)
{
  m_data[0] += rhs;
  m_data[1] += rhs;

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator+=(Mat<2, 2, T> const& rhs)
{
  m_data[0] += rhs.m_data[0];
  m_data[1] += rhs.m_data[1];

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator-=(T rhs)
{
  m_data[0] -= rhs;
  m_data[1] -= rhs;

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator-=(Mat<2, 2, T> const& rhs)
{
  m_data[0] -= rhs.m_data[0];
  m_data[1] -= rhs.m_data[1];

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator*=(T rhs)
{
  m_data[0] *= rhs;
  m_data[1] *= rhs;

  return *this;
}

template <typename T>
Mat<2, 2, T>& Mat<2, 2, T>::operator/=(T rhs)
{
  m_data[0] /= rhs;
  m_data[1] /= rhs;

  return *this;
}

template <typename T>
Mat<2, 2, T> Mat<2, 2, T>::operator-() const
{
  return { -m_data[0], -m_data[1] };
}


// Accessors
template <typename T>
typename Mat<2, 2, T>::ValueType& Mat<2, 2, T>::operator[](unsigned rhs)
{
  return *(m_data + rhs);
}

template <typename T>
typename Mat<2, 2, T>::ValueType const& Mat<2, 2, T>::operator[](unsigned rhs) const
{
  return *(m_data + rhs);
}

template <typename T>
T& Mat<2, 2, T>::At(unsigned col, unsigned row)
{
  if ((col > 1 || col < 0) || (row > 1 || row < 0))
  {
    throw std::out_of_range("At: out of range");
  }

  return m_data[row][col];
}

template <typename T>
T const& Mat<2, 2, T>::At(unsigned col, unsigned row) const
{
  if ((col > 1 || col < 0) || (row > 1 || row < 0))
  {
    throw std::out_of_range("At (const): out of range");
  }

  return m_data[row][col];
}


// Member Functions
template <typename T>
typename Mat<2, 2, T>::ValueType Mat<2, 2, T>::GetCol(unsigned col) const
{
  if (col > 1 || col < 0)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return { m_data[0][col], m_data[1][col] };
}

template <typename T>
typename Mat<2, 2, T>::ValueType Mat<2, 2, T>::GetRow(unsigned row) const
{
  if (row > 1 || row < 0)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return m_data[row];
}

template <typename T>
void Mat<2, 2, T>::Transpose()
{
  std::swap(m_data[0].y, m_data[1].x);
}


// Non-member operator overloads
template <typename T>
Mat<2, 2, T> operator+(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs)
{
  return { lhs[0] + rhs[0], lhs[1] + rhs[1] };
}

template <typename T>
Mat<2, 2, T> operator+(Mat<2, 2, T> const& lhs, T rhs)
{
  return { lhs[0] + rhs, lhs[1] + rhs };
}

template <typename T>
Mat<2, 2, T> operator-(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs)
{
  return { lhs[0] - rhs[0], lhs[1] - rhs[1] };
}

template <typename T>
Mat<2, 2, T> operator-(Mat<2, 2, T> const& lhs, T rhs)
{
  return { lhs[0] - rhs, lhs[1] - rhs };
}

template <typename T>
Mat<2, 2, T> operator*(T lhs, Mat<2, 2, T> const& rhs)
{
  return { lhs * rhs[0], lhs * rhs[1] };
}

template <typename T>
Mat<2, 2, T> operator*(Mat<2, 2, T> const& lhs, T rhs)
{
  return { lhs[0] * rhs, lhs[1] * rhs }; 
}

template <typename T>
Vec<2, T> operator*(Mat<2, 2, T> const& lhs, Vec<2, T> const& rhs)
{
  return { Dot(lhs[0], rhs), Dot(lhs[1], rhs) };
}

template <typename T>
Mat<2, 2, T> operator/(Mat<2, 2, T> const& lhs, T rhs)
{
  return { lhs[0] / rhs, lhs[1] / rhs };
}

template <typename T>
bool operator==(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs)
{
  return (lhs[0] == rhs[0] && lhs[1] == rhs[1]);
}

template <typename T>
bool operator!=(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs)
{
  return !(lhs[0] == rhs[0] && lhs[1] == rhs[1]);
}