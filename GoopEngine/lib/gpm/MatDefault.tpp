// Constructors
template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>::Mat()
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i]= ValueType();
  }
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>::Mat(Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] = rhs[i];
  }
}

// Operator overloads
template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator=(Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] = rhs[i];
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator+=(T rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] += rhs;
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator+=(Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] += rhs[i];
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator-=(T rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] -= rhs;
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator-=(Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] -= rhs[i];
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator*=(T rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] *= rhs;
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator/=(T rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    m_data[i] /= rhs;
  }

  return *this;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> Mat<Rows, Cols, T>::operator-() const
{
  Mat<Rows, Cols, T> temp{ *this };
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = -temp[i];
  }

  return temp;
}

// Accessors
template <unsigned Rows, unsigned Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType& Mat<Rows, Cols, T>::operator[](unsigned rhs)
{
  return *(m_data + rhs);
}

template <unsigned Rows, unsigned Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType const& Mat<Rows, Cols, T>::operator[](unsigned rhs) const
{
  return *(m_data + rhs);
}

template <unsigned Rows, unsigned Cols, typename T>
T& Mat<Rows, Cols, T>::At(unsigned row, unsigned col)
{
  return m_data[col][row];
}

template <unsigned Rows, unsigned Cols, typename T>
T const& Mat<Rows, Cols, T>::At(unsigned row, unsigned col) const
{
  return m_data[col][row];
}

// Member Functions
template <unsigned Rows, unsigned Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType Mat<Rows, Cols, T>::GetCol(unsigned col) const
{
  if (col >= Cols)
  {
    throw std::out_of_range("call to GetCol: out of range");
  }

  return m_data[col];
}

template <unsigned Rows, unsigned Cols, typename T>
Vec<Cols, T> Mat<Rows, Cols, T>::GetRow(unsigned row) const
{
  if (row >= Rows)
  {
    throw std::out_of_range("call to GetRow: out of range");
  }

  return { m_data[0][row], m_data[1][row], m_data[2][row] };
}

template <unsigned Rows, unsigned Cols, typename T>
void Mat<Rows, Cols, T>::Transpose()
{
  static_assert(Rows == Cols, "member function Transpose() can only be called on a square matrix!");
  
  Mat<Rows, Cols, T> temp{ *this };
  for (unsigned i{}; i < Cols; ++i)
  {
    ValueType currCol{ GetCol(i) };
    for (unsigned j{}; j < Rows; ++j)
    {
      temp[j][i] = currCol[j];
    }
  }
  *this = temp;
}

// Non-member operator overloads
template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] + rhs[i];
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] + rhs;
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] - rhs[i];
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] - rhs;
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator*(T lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs * rhs[i];
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator*(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = rhs * lhs[i];
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
Mat<Rows, Cols, T> operator/(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (unsigned i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] / rhs;
  }

  return temp;
}

template <unsigned Rows, unsigned Cols, typename T>
bool operator==(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <unsigned Rows, unsigned Cols, typename T>
bool operator!=(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (unsigned i{}; i < Cols; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}