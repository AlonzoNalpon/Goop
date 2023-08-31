// Constructors
template <typename T>
Mat<3, 3, T>::Mat()
{
  m_data[0] = m_data[1] = m_data[2] = ValueType();
}

template <typename T>
Mat<3, 3, T>::Mat(Mat<3, 3, T> const& rhs)
{
  for (unsigned i{}; i < 3; ++i)
  {
    m_data[i] = rhs[i];
  }
}

template <typename T>
Mat<3, 3, T>::Mat(ValueType const& c0, ValueType const& c1,  ValueType const& c2) 
{
  m_data[0] = c0;
  m_data[1] = c1;
  m_data[2] = c2;
}

// Operator overloads
template <typename T>
Mat<3, 3, T>& Mat<3, 3, T>::operator=(Mat<3, 3, T> const& rhs)
{
  for (unsigned i{}; i < 3; ++i)
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
  for (unsigned i{}; i < 3; ++i)
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
  for (unsigned i{}; i < 3; ++i)
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
typename Mat<3, 3, T>::ValueType& Mat<3, 3, T>::operator[](unsigned rhs)
{
  return *(m_data + rhs);
}

template <typename T>
typename Mat<3, 3, T>::ValueType const& Mat<3, 3, T>::operator[](unsigned rhs) const
{
  return *(m_data + rhs);
}

template <typename T>
T& Mat<3, 3, T>::At(unsigned row, unsigned col)
{
  return m_data[col][row];
}

template <typename T>
T const& Mat<3, 3, T>::At(unsigned row, unsigned col) const
{
  return m_data[col][row];
}

// Member Functions
template <typename T>
typename Mat<3, 3, T>::ValueType Mat<3, 3, T>::GetCol(unsigned col) const
{
  if (col > 2)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return m_data[col];
}

template <typename T>
typename Mat<3, 3, T>::ValueType Mat<3, 3, T>::GetRow(unsigned row) const
{
  if (row > 2)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return { m_data[0][row], m_data[1][row], m_data[2][row] };
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
  return { Dot(lhs.GetRow(0), rhs), Dot(lhs.GetRow(1), rhs), Dot(lhs.GetRow(2), rhs) };
}

template <typename T>
Mat<3, 3, T> operator/(Mat<3, 3, T> const& lhs, T rhs)
{
  return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs };
}

template <typename T>
bool operator==(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  for (unsigned i{}; i < 3; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <typename T>
bool operator!=(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs)
{
  for (unsigned i{}; i < 3; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}