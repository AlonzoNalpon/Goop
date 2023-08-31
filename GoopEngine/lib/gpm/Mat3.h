#ifndef GOOP_MAT3_H
#define GOOP_MAT3_H

namespace gpm
{
  template <typename T>
  struct Mat<3, 3, T>
	{
		using ValueType = Vec<3, T>;

    ValueType m_data[3];

    // Constructors
    Mat();
    Mat(Mat<3, 3, T> const& rhs);
    Mat(ValueType const& c0, ValueType const& c1, ValueType const& c2);

		// Accessors
		T& At(unsigned row, unsigned col);
		T const& At(unsigned row, unsigned col) const;
		ValueType& operator[](unsigned rhs);
		ValueType const& operator[](unsigned rhs) const;

    // Operator overloads
		Mat<3, 3, T>& operator=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator+=(T rhs);
		Mat<3, 3, T>& operator+=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator-=(T rhs);
		Mat<3, 3, T>& operator-=(Mat<3, 3, T> const& rhs);
		Mat<3, 3, T>& operator*=(T rhs);
		Mat<3, 3, T>& operator/=(T rhs);
		Mat<3, 3, T> operator-() const;
        
    //  Member functions
		ValueType GetCol(unsigned col) const;
		ValueType GetRow(unsigned row) const;
		void Transpose();
  };

  #include "Mat3.tpp"

	// Non-member operator overloads
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