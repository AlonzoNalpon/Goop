#ifndef GOOP_MAT3_H
#define GOOP_MAT3_H

namespace gpm
{
  template <typename T>
  struct Mat<3, 3, T>
	{
		using ValueType = Vec<3, T>;

    ValueType m_data[3];

    // CONSTRUCTORS
    Mat();

		/*!***********************************************************************
		\brief
			Non-default constructor taking in 9 elements. They should be specified
			in row-major order.
		*************************************************************************/
		Mat( T m00, T m01, T m02,
				 T m10, T m11, T m12,
				 T m20, T m21, T m22 );

		/*!***********************************************************************
		\brief
			Constructor taking in 3 sized-3 vectors with each vector corresponding
			to a column.
		*************************************************************************/
    Mat(ValueType const& col0, ValueType const& col1, ValueType const& col2);
		Mat(Mat<3, 3, T> const& rhs);

		// ACCESSORS
		/*!***********************************************************************
		\brief
			Accesses the element of the matrix at row, col (i.e x, y).
			Note that matrix is in row-major order.
			If out of range, throws std::out_of_range exception.
		\param[in] row
			The row of the element [0,2]
		\param[in] col
			The column of the element [0,2]
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
			The row of the element [0,2]
		\param[in] col
			The column of the element [0,2]
		\return
			The element of the matrix (const) at (row, col)
		*************************************************************************/
		T const& At(size_type row, size_type col) const;

		/*!***********************************************************************
		\brief
			Accesses a column of the matrix in the form of a vector.
			Note that matrix is in row-major order.
		\param[in] rhs
			The column to return [0,2]
		\return
			Vector corresponding to the specified column of the matrix
		*************************************************************************/
		ValueType& operator[](size_type rhs);

		/*!***********************************************************************
		\brief
			Accesses a column of the matrix in the form of a vector (const).
			Note that matrix is in row-major order.
		\param[in] rhs
			The column to return [0,2]
		\return
			Vector (const) corresponding to the specified column of the matrix
		*************************************************************************/
		ValueType const& operator[](size_type rhs) const;

    // OPERATOR OVERLOADS
		Mat<3, 3, T>& operator=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator+=(T rhs);
		Mat<3, 3, T>& operator+=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator-=(T rhs);
		Mat<3, 3, T>& operator-=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator*=(T rhs);
		Mat<3, 3, T>& operator/=(T rhs);
		Mat<3, 3, T> operator-() const;
        
    // MEMBER FUNCTIONS
		/*!***********************************************************************
		\brief
			Returns a column of the matrix in the form of an rvalue vector.
			Note that matrix is in row-major order.
			If out of range, throws std::out_of_range exception.
		\param[in] col
			The column to return [0,2]
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
			The column to return [0,2]
		\return
			Vector corresponding to the specified row of the matrix
		*************************************************************************/
		ValueType GetRow(size_type row) const;

		/*!***********************************************************************
		\brief
			Transposes the current matrix
		*************************************************************************/
		void Transpose();
  };

  #include "Mat3.tpp"

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
	Vec<3, T> operator*(Mat<3, 3, T> const& lhs, Vec<3, T> const& rhs);
	template <typename T>
	Mat<3, 3, T> operator/(Mat<3, 3, T> const& lhs, T rhs);
	template <typename T>
	bool operator==(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);
	template <typename T>
	bool operator!=(Mat<3, 3, T> const& lhs, Mat<3, 3, T> const& rhs);

} // namespace gpm

#endif