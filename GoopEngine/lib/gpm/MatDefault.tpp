// Constructors
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat()
{
  for (size_type i{}; i < Rows; ++i)
  {
    m_data[i]= ValueType();
  }
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1)
{
  static_assert(Rows == 2, "Mat<Rows, Cols, T>: Constructor can only be called on a 2-row matrix");

  m_data[0] = row0;
  m_data[1] = row1;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1, Vec<Cols, T> const& row2)
{
  static_assert(Rows == 3, "Mat<Rows, Cols, T>: Constructor can only be called on a 3-row matrix");

  m_data[0] = row0;
  m_data[1] = row1;
  m_data[2] = row2;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1, 
		  Vec<Cols, T> const& row2, Vec<Cols, T> const& row3)
{
  static_assert(Rows == 4, "Mat<Rows, Cols, T>: Constructor can only be called on a 4-row matrix");
  
  m_data[0] = row0;
  m_data[1] = row1;
  m_data[2] = row2;
  m_data[3] = row3;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T>::Mat(Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Rows; ++i)
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
T& Mat<Rows, Cols, T>::At(size_type col, size_type row)
{
  if ((col >= Cols || col < 0) || (row >= Rows || row < 0))
  {
    throw std::out_of_range("At: out of range");
  }
  
  return m_data[row][col];
}

template <size_type Rows, size_type Cols, typename T>
T const& Mat<Rows, Cols, T>::At(size_type col, size_type row) const
{
  if ((col >= Cols || col < 0) || (row >= Rows || row < 0))
  {
    throw std::out_of_range("At (const): out of range");
  }
  
  return m_data[row][col];
}

// Member Functions
template <size_type Rows, size_type Cols, typename T>
typename Mat<Rows, Cols, T>::ValueType Mat<Rows, Cols, T>::GetCol(size_type col) const
{
  if (col >= Cols)
  {
    throw std::out_of_range("GetCol: out of range");
  }

  return { m_data[0][col], m_data[1][col], m_data[2][col] };
}

template <size_type Rows, size_type Cols, typename T>
Vec<Cols, T> Mat<Rows, Cols, T>::GetRow(size_type row) const
{
  if (row >= Rows)
  {
    throw std::out_of_range("GetRow: out of range");
  }

  return m_data[row];
}

// Non-member operator overloads
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs[i] + rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs[i] + rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs[i] - rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs[i] - rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(T lhs, Mat<Rows, Cols, T> const& rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs * rhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = rhs * lhs[i];
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Vec<Rows, T> operator*(Mat<Rows, Cols, T> const& lhs, Vec<Rows, T> const& rhs)
{
  Vec<Rows, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = Dot(lhs[i], rhs);
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator/(Mat<Rows, Cols, T> const& lhs, T rhs)
{
  Mat<Rows, Cols, T> temp{};
  for (size_type i{}; i < Rows; ++i)
  {
    temp[i] = lhs[i] / rhs;
  }

  return temp;
}

template <size_type Rows, size_type Cols, typename T>
bool operator==(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Rows; ++i)
  {
    if (lhs[i] != rhs[i]) { return false; }
  }

  return true;
}

template <size_type Rows, size_type Cols, typename T>
bool operator!=(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs)
{
  for (size_type i{}; i < Rows; ++i)
  {
    if (lhs[i] != rhs[i]) { return true; }
  }

  return false;
}