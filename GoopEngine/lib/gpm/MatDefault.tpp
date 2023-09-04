// Constructors
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat()
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i]= ValueType();
  }
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1)
{
  static_assert(Cols == 2, "Mat<Rows, Cols, T>: Constructor can only be called on a 2-column matrix");

  m_data[0] = c0;
  m_data[1] = c1;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1,  Vec<Rows, T> const& c2)
{
  static_assert(Cols == 3, "Mat<Rows, Cols, T>: Constructor can only be called on a 3-column matrix");

  m_data[0] = c0;
  m_data[1] = c1;
  m_data[2] = c2;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1,
    Vec<Rows, T> const& c2,  Vec<Rows, T> const& c3)
{
  static_assert(Cols == 4, "Mat<Rows, Cols, T>: Constructor can only be called on a 4-column matrix");

  m_data[0] = c0;
  m_data[1] = c1;
  m_data[2] = c2;
  m_data[3] = c3;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] = rhs[i];
  }
}

// Operator overloads
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator=(Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] = rhs[i];
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator+=(T rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] += rhs;
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator+=(Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] += rhs[i];
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator-=(T rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] -= rhs;
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator-=(Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] -= rhs[i];
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator*=(T rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] *= rhs;
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>& Mat<Rows, Cols, T>::operator/=(T rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    m_data[i] /= rhs;
  }

  return *this;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> Mat<Rows, Cols, T>::operator-() const
{
  Mat<Rows, Cols, T> temp{ *this };
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = -temp[i];
  }

  return temp;
}

// Accessors
template <size_type Rows, size_type Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType& Mat<Rows, Cols, T>::operator[](size_type rhs)
{
  return *(m_data + rhs);
}

template <size_type Rows, size_type Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType const& Mat<Rows, Cols, T>::operator[](size_type rhs) const
{
  return *(m_data + rhs);
}

template <size_type Rows, size_type Cols, typename T>
T& Mat<Rows, Cols, T>::At(size_type row, size_type col)
{
  return m_data[col][row];
}

template <size_type Rows, size_type Cols, typename T>
T const& Mat<Rows, Cols, T>::At(size_type row, size_type col) const
{
  return m_data[col][row];
}

// Member Functions
template <size_type Rows, size_type Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType Mat<Rows, Cols, T>::GetCol(size_type col) const
{
  if (col >= Cols)
  {
    throw std::out_of_range("call to GetCol: out of range");
  }

  return m_data[col];
}

template <size_type Rows, size_type Cols, typename T>
Vec<Cols, T> Mat<Rows, Cols, T>::GetRow(size_type row) const
{
  if (row >= Rows)
  {
    throw std::out_of_range("call to GetRow: out of range");
  }

  return { m_data[0][row], m_data[1][row], m_data[2][row] };
}

template <size_type Rows, size_type Cols, typename T>
void Mat<Rows, Cols, T>::Transpose()
{
  static_assert(Rows == Cols, "member function Transpose() can only be called on a square matrix!");
  
  Mat<Rows, Cols, T> temp{ *this };
  for (size_type i{}; i < Cols; ++i)
  {
    ValueType currCol{ GetCol(i) };
    for (size_type j{}; j < Rows; ++j)
    {
      temp[j][i] = currCol[j];
    }
  }
  *this = temp;
}

// Non-member operator overloads
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] + rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] + rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] - rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] - rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(T lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs * rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = rhs * lhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator/(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Cols; ++i)
  {
    temp[i] = lhs[i] / rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
bool operator==(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <size_type Rows, size_type Cols, typename T>
bool operator!=(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Cols; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}