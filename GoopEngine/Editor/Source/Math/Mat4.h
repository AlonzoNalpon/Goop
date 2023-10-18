/*!*********************************************************************
\file   Mat4.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Specialized definition for 4x4 Matrices from GEM library.
				Contains accessors, constructors, member and non-member
				operator overloads.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_MAT4_H
#define GOOP_MAT4_H

namespace GE
{
	namespace Math
	{
		template <typename T>
		struct Mat<4, 4, T>
		{
			using ValueType = Vec<4, T>;

			ValueType m_data[4];

			// Constructors
			Mat();

			/*!***********************************************************************
			\brief
				Constructor taking in 4 sized-4 vectors with each vector corresponding
				to a row.
			*************************************************************************/
			Mat(ValueType const& row0, ValueType const& row1,
				ValueType const& row2, ValueType const& row3);
			Mat(Mat<4, 4, T> const& rhs);

			// ACCESSORS
			/*!***********************************************************************
			\brief
				Accesses the element of the matrix at col, row (i.e x, y).
				Note that matrix is in row-major order.
				If out of range, throws std::out_of_range exception.
			\param[in] col
				The column of the element (horizontal axis) [0,3]
			\param[in] row
				The row of the element (vertical axis) [0,3]
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
				The column of the element (horizontal axis) [0,3]
			\param[in] row
				The row of the element (vertical axis) [0,3]
			\return
				The element of the matrix (const) at (col, row)
			*************************************************************************/
			T const& At(size_type col, size_type row) const;

			/*!***********************************************************************
			\brief
				Accesses a row of the matrix in the form of a vector.
				Note that matrix is in row-major order.
			\param[in] rhs
				The column to return [0,3]
			\return
				Vector corresponding to the specified row of the matrix
			*************************************************************************/
			ValueType& operator[](size_type rhs);

			/*!***********************************************************************
			\brief
				Accesses a row of the matrix in the form of a vector (const).
				Note that matrix is in row-major order.
			\param[in] rhs
				The column to return [0,3]
			\return
				Vector (const) corresponding to the specified row of the matrix
			*************************************************************************/
			ValueType const& operator[](size_type rhs) const;

			// OPERATOR OVERLOADS
			Mat<4, 4, T>& operator=(Mat<4, 4, T> const& rhs);
			Mat<4, 4, T>& operator+=(T rhs);
			Mat<4, 4, T>& operator+=(Mat<4, 4, T> const& rhs);
			Mat<4, 4, T>& operator-=(T rhs);
			Mat<4, 4, T>& operator-=(Mat<4, 4, T> const& rhs);
			Mat<4, 4, T>& operator*=(T rhs);
			Mat<4, 4, T>& operator/=(T rhs);
			Mat<4, 4, T> operator-() const;

			//  MEMBER FUNCTIONS
			/*!***********************************************************************
			\brief
				Returns a column of the matrix in the form of an rvalue vector.
				Note that matrix is in row-major order.
				If out of range, throws std::out_of_range exception.
			\param[in] col
				The column to return [0,3]
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
				The column to return [0,3]
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

#include "Mat4.tpp"

		// NON-MEMBER OPERATOR OVERLOADS
		template <typename T>
		Mat<4, 4, T> operator+(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);
		template <typename T>
		Mat<4, 4, T> operator+(Mat<4, 4, T> const& lhs, T rhs);
		template <typename T>
		Mat<4, 4, T> operator-(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);
		template <typename T>
		Mat<4, 4, T> operator-(Mat<4, 4, T> const& lhs, T rhs);
		template <typename T>
		Mat<4, 4, T> operator*(T lhs, Mat<4, 4, T> const& rhs);
		template <typename T>
		Mat<4, 4, T> operator*(Mat<4, 4, T> const& lhs, T rhs);
		template <typename T>
		Vec<4, T> operator*(Mat<4, 4, T> const& lhs, Vec<4, T> const& rhs);
		template <typename T>
		Mat<4, 4, T> operator/(Mat<4, 4, T> const& lhs, T rhs);
		template <typename T>
		bool operator==(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);
		template <typename T>
		bool operator!=(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);
	}
} // namespace GE

#endif