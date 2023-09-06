#ifndef GOOP_MAT_DEFAULT_H
#define GOOP_MAT_DEFAULT_H

namespace GPM
{
  template <size_type Rows, size_type Cols, typename T>
  struct Mat
  {
    // ensure only rows/cols of sizes 2, 3, 4 are instantiated
    static_assert(!(Rows < 2 || Rows > 4) || (Cols < 2 || Cols > 4), 
      "Mat<size_type Row, size_type Col, typename T>: only rows/cols of 2, 3 and 4 are supported!");
    using ValueType = Vec<Cols, T>;

    ValueType m_data[Rows];

    // CONSTRUCTORS
    Mat();
    /*!***********************************************************************
    \brief
      2x<Cols> constructor taking in 2 vectors of size <Cols> with each
      vector corresponding to a row.
    *************************************************************************/
    Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1);

    /*!***********************************************************************
    \brief
      3x<Cols> constructor taking in 3 vectors of size <Cols> with each
      vector corresponding to a row.
    *************************************************************************/
    Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1, Vec<Cols, T> const& row2);

    /*!***********************************************************************
    \brief
      4x<Cols> constructor taking in 4 vectors of size <Cols> with each
      vector corresponding to a row.
    *************************************************************************/
    Mat(Vec<Cols, T> const& row0, Vec<Cols, T> const& row1, 
        Vec<Cols, T> const& row2, Vec<Cols, T> const& row3);
    Mat(Mat<Rows, Cols, T> const& rhs);

    // ACCESSORS
    /*!***********************************************************************
    \brief
      Accesses the element of the matrix at col, row (i.e x, y).
      Note that matrix is in row-major order.
      If out of range, throws std::out_of_range exception.
    \param[in] col
      The column of the element (horizontal axis) [0,Cols)
    \param[in] row
      The row of the element (vertical axis) [0,Rows)
    \return
      The element of the matrix at (col, row)
    *************************************************************************/
    T& At(size_type col, size_type row);

    /*!***********************************************************************
    \brief
      Accesses the element of the matrix (const) at col, row (i.e x, y).
      Note that matrix is in row-major order.
      If out of range, throws std::out_of_range exception.
    \param[in] col
      The column of the element (horizontal axis) [0,Cols)
    \param[in] row
      The row of the element (vertical axis) [0,Rows)
    \return
      The element of the matrix (const) at (col, row)
    *************************************************************************/
    T const& At(size_type col, size_type row) const;

    /*!***********************************************************************
    \brief
      Accesses a row of the matrix in the form of a vector.
      Note that matrix is in row-major order.
    \param[in] rhs
      The row to return [0,Rows]
    \return
      Vector corresponding to the specified row of the matrix
    *************************************************************************/
    ValueType& operator[](size_type rhs);

    /*!***********************************************************************
    \brief
      Accesses a row of the matrix in the form of a vector (const).
      Note that matrix is in row-major order.
    \param[in] rhs
      The row to return [0,Rows]
    \return
      Vector (const) corresponding to the specified row of the matrix
    *************************************************************************/
    ValueType const& operator[](size_type rhs) const;

    // Operator overloads
    Mat<Rows, Cols, T>& operator=(Mat<Rows, Cols, T> const& rhs);
    Mat<Rows, Cols, T>& operator+=(T rhs);
    Mat<Rows, Cols, T>& operator+=(Mat<Rows, Cols, T> const& rhs);
    Mat<Rows, Cols, T>& operator-=(T rhs);
    Mat<Rows, Cols, T>& operator-=(Mat<Rows, Cols, T> const& rhs);
    Mat<Rows, Cols, T>& operator*=(T rhs);
    Mat<Rows, Cols, T>& operator/=(T rhs);
    Mat<Rows, Cols, T> operator-() const;
          
    //  MEMBER FUNCTIONS
    /*!***********************************************************************
    \brief
      Returns a column of the matrix in the form of an rvalue vector.
      Note that matrix is in row-major order.
      If out of range, throws std::out_of_range exception.
    \param[in] col
      The column to return [0,Cols]
    \return
      Vector corresponding to the specified column of the matrix
    *************************************************************************/
    ValueType GetCol(size_type col) const;

    /*!***********************************************************************
    \brief
      Returns a row of the matrix in the form of an rvalue vector.
      Note that matrix is in row-major order.
      If out of range, throws std::out_of_range exception.
    \param[in] row
      The row to return [0,Rows]
    \return
      Vector corresponding to the specified row of the matrix
    *************************************************************************/
    Vec<Cols, T> GetRow(size_type row) const;
  };

#include "MatDefault.tpp"

// NON-MEMBER OPERATOR OVERLOADS
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator+(Mat<Rows, Cols, T> const& lhs, T rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator-(Mat<Rows, Cols, T> const& lhs, T rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(T lhs, Mat<Rows, Cols, T> const& rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator*(Mat<Rows, Cols, T> const& lhs, T rhs);
template <size_type Rows, size_type Cols, typename T>
Vec<Rows, T> operator*(Mat<Rows, Cols, T> const& lhs, Vec<Rows, T> const& rhs);
template <size_type Rows, size_type Cols, typename T>
Mat<Rows, Cols, T> operator/(Mat<Rows, Cols, T> const& lhs, T rhs);
template <size_type Rows, size_type Cols, typename T>
bool operator==(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs);
template <size_type Rows, size_type Cols, typename T>
bool operator!=(Mat<Rows, Cols, T> const& lhs, Mat<Rows, Cols, T> const& rhs);

} // namespace GPM

#endif