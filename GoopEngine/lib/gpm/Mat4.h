#ifndef GOOP_MAT4_H
#define GOOP_MAT4_H

namespace gpm
{
  template <typename T>
  struct Mat<4, 4, T>
	{
		using ValueType = Vec<4, T>;

    ValueType m_data[4];

    // Constructors
    Mat();
    Mat(Mat<4, 4, T> const& rhs);
    Mat(ValueType const& c0, ValueType const& c1, ValueType const& c2, ValueType const& c3);

		// Accessors
		T& At(unsigned row, unsigned col);
		T const& At(unsigned row, unsigned col) const;
		ValueType& operator[](unsigned rhs);
		ValueType const& operator[](unsigned rhs) const;

    // Operator overloads
		Mat<4, 4, T>& operator=(Mat<4, 4, T> const& rhs);
		Mat<4, 4, T>& operator+=(T rhs);
		Mat<4, 4, T>& operator+=(Mat<4, 4, T> const& rhs);
		Mat<4, 4, T>& operator-=(T rhs);
		Mat<4, 4, T>& operator-=(Mat<4, 4, T> const& rhs);
		Mat<4, 4, T>& operator*=(T rhs);
		Mat<4, 4, T>& operator/=(T rhs);
		Mat<4, 4, T> operator-() const;
        
    //  Member functions
		ValueType GetCol(unsigned col) const;
		ValueType GetRow(unsigned row) const;
		void Transpose();
  };

  #include "Mat4.tpp"

	// Non-member operator overloads
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
	Mat<4, 4, T> operator/(Mat<4, 4, T> const& lhs, T rhs);
	template <typename T>
	bool operator==(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);
	template <typename T>
	bool operator!=(Mat<4, 4, T> const& lhs, Mat<4, 4, T> const& rhs);

} // namespace gpm

#endif