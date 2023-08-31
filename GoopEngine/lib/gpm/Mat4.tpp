// Constructors
template <typename T>
Mat<4, 4, T>::Mat()
{
  m_data[0] = m_data[1] = m_data[2] = m_data[3] = ValueType();
}

template <typename T>
Mat<4, 4, T>::Mat(Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    m_data[i] = rhs[i];
  }
}

template <typename T>
Mat<4, 4, T>::Mat(ValueType const& c0, ValueType const& c1, ValueType const& c2, ValueType const& c3) 
{
  m_data[0] = c0;
  m_data[1] = c1;
  m_data[2] = c2;
  m_data[3] = c3;
}

// Operator overloads
template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator=(Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    m_data[i] = rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator+=(T rhs)
{
  m_data[0] += rhs;
  m_data[1] += rhs;
  m_data[2] += rhs;
  m_data[3] += rhs;

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator+=(Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    m_data[i] += rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator-=(T rhs)
{
  m_data[0] -= rhs;
  m_data[1] -= rhs;
  m_data[2] -= rhs;
  m_data[3] -= rhs;

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator-=(Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    m_data[i] -= rhs[i];
  }

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator*=(T rhs)
{
  m_data[0] *= rhs;
  m_data[1] *= rhs;
  m_data[2] *= rhs;
  m_data[3] *= rhs;

  return *this;
}

template <typename T>
Mat<4, 4, T>& Mat<4, 4, T>::operator/=(T rhs)
{
  m_data[0] /= rhs;
  m_data[1] /= rhs;
  m_data[2] /= rhs;
  m_data[3] /= rhs;

  return *this;
}

template <typename T>
Mat<4, 4, T> Mat<4, 4, T>::operator-() const
{
  return { -m_data[0], -m_data[1], -m_data[2], -m_data[3] };
}

// Accessors
template <typename T>
typename Mat<4, 4, T>::ValueType& Mat<4, 4, T>::operator[](unsigned rhs)
{
  return *(m_data + rhs);
}

template <typename T>
typename Mat<4, 4, T>::ValueType const& Mat<4, 4, T>::operator[](unsigned rhs) const
{
  return *(m_data + rhs);
}

template <typename T>
T& Mat<4, 4, T>::At(unsigned row, unsigned col)
{
  return m_data[col][row];
}

template <typename T>
T const& Mat<4, 4, T>::At(unsigned row, unsigned col) const
{
  return m_data[col][row];
}

// Member Functions
template <typename T>
typename Mat<4, 4, T>::ValueType Mat<4, 4, T>::GetCol(unsigned col) const
{
  if (col > 3)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return m_data[col];
}

template <typename T>
typename Mat<4, 4, T>::ValueType Mat<4, 4, T>::GetRow(unsigned row) const
{
  if (row > 3)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return { m_data[0][row], m_data[1][row], m_data[2][row], m_data[3][row] };
}

template <typename T>
void Mat<4, 4, T>::Transpose()
{
  Mat<4, 4, T> temp{ *this };
  temp[0] = GetRow(0);
  temp[1] = GetRow(1);
  temp[2] = GetRow(2);
  temp[3] = GetRow(3);

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
  return { Dot(lhs.GetRow(0), rhs), Dot(lhs.GetRow(1), rhs), 
           Dot(lhs.GetRow(2), rhs), Dot(lhs.GetRow(3), rhs) };
}

template <typename T>
Mat<4, 4, T> operator/(Mat<4, 4, T> const& lhs, T rhs)
{
  return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs, lhs[3] / rhs };
}

template <typename T>
bool operator==(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <typename T>
bool operator!=(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs)
{
  for (unsigned i{}; i < 4; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}