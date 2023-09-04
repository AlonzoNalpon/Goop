#ifndef GOOP_MAT_DEFAULT_H
#define GOOP_MAT_DEFAULT_H

namespace gpm
{
  template <size_type Rows, size_type Cols, typename T>
  struct Mat
  {
    // ensure only rows/cols of sizes 2, 3, 4 are instantiated
    static_assert(!(Rows < 2 || Rows > 4) || (Cols < 2 || Cols > 4), 
      "Mat<size_type Row, size_type Col, typename T>: only rows/cols of 2, 3 and 4 are supported!");
    using ValueType = Vec<Rows, T>;

    ValueType m_data[Cols];

    // CONSTRUCTORS
    Mat();
		/*!***********************************************************************
		\brief
			2x<Rows> constructor taking in 2 vectors of size <Rows> with each
			vector corresponding to a column.
		*************************************************************************/
		Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1);

		/*!***********************************************************************
		\brief
			3x<Rows> constructor taking in 3 vectors of size <Rows> with each
			vector corresponding to a column.
		*************************************************************************/
		Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1, Vec<Rows, T> const& c2);

		/*!***********************************************************************
		\brief
			4x<Rows> constructor taking in 4 vectors of size <Rows> with each
			vector corresponding to a column.
		*************************************************************************/
		Mat(Vec<Rows, T> const& c0, Vec<Rows, T> const& c1, 
			  Vec<Rows, T> const& c2, Vec<Rows, T> const& c3);
    Mat(Mat<Rows, Cols, T> const& rhs);

		// ACCESSORS
		/*!***********************************************************************
		\brief
			Accesses the element of the matrix at row, col (i.e x, y).
			Note that matrix is in row-major order.
			If out of range, throws std::out_of_range exception.
		\param[in] row
			The row of the element [0,Cols]
		\param[in] col
			The column of the element [0,Rows]
		\return
			The element of the matrix at (row, col)
		*************************************************************************/
		T& At(size_type row, size_type col);

		/*!***********************************************************************
		\brief
			Accesses the element of the matrix (const) at row, col (i.e x, y).
			Note that matrix is in row-major order.
			If out of range, throws std::out_of_range exception.
		\param[in] row
			The row of the element [0,Cols]
		\param[in] col
			The column of the element [0,Rows]
		\return
			The element of the matrix (const) at (row, col)
		*************************************************************************/
		T const& At(size_type row, size_type col) const;

		/*!***********************************************************************
		\brief
			Accesses a column of the matrix in the form of a vector.
			Note that matrix is in row-major order.
		\param[in] rhs
			The column to return [0,Cols]
		\return
			Vector corresponding to the specified column of the matrix
		*************************************************************************/
		ValueType& operator[](size_type rhs);

		/*!***********************************************************************
		\brief
			Accesses a column of the matrix in the form of a vector (const).
			Note that matrix is in row-major order.
		\param[in] rhs
			The column to return [0,Rows]
		\return
			Vector (const) corresponding to the specified column of the matrix
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
			The column to return [0,Rows]
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
			The column to return [0,Cols]
		\return
			Vector corresponding to the specified row of the matrix
		*************************************************************************/
		Vec<Cols, T> GetRow(size_type row) const;

		/*!***********************************************************************
		\brief
			Transposes the current matrix
		*************************************************************************/
		void Transpose();
  };

  #include "MatDefault.tpp"

	// NON-MEMBER OPERATOR OVERLOADS
	template <typename T>
	Mat<3, 3, T> operator+(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);
	template <typename T>
	Mat<3, 3, T> operator+(Mat<3, 3, T> const& lhs, T rhs);
	template <typename T>
	Mat<3, 3, T> operator-(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);
	template <typename T>
	Mat<3, 3, T> operator-(Mat<3, 3, T> const& lhs, T rhs);
	template <typename T>
	Mat<3, 3, T> operator*(T lhs, Mat<3, 3, T> const& rhs);
	template <typename T>
	Mat<3, 3, T> operator*(Mat<3, 3, T> const& lhs, T rhs);
	template <typename T>
	Mat<3, 3, T> operator/(Mat<3, 3, T> const& lhs, T rhs);
	template <typename T>
	bool operator==(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);
	template <typename T>
	bool operator!=(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);

} // namespace gpm

#endif