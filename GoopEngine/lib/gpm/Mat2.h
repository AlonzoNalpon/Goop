#ifndef GOOP_MAT2_H
#define GOOP_MAT2_H

namespace gpm 
{
  template <typename T>
  struct Mat<2, 2, T>
	{
    using ValueType = Vec<2, T>;

    ValueType m_data[2];

    // Constructors
    Mat();
    Mat(T m00, T m01, T m10, T m11);
    Mat(Mat<2, 2, T> const& rhs);
    Mat(ValueType const& col0, ValueType const& col1);

		// Accessors
		T& At(unsigned row, unsigned col);
		T const& At(unsigned row, unsigned col) const;
		ValueType& operator[](unsigned rhs);
		ValueType const& operator[](unsigned rhs) const;

    // Operator overloads
    Mat<2, 2, T>& operator=(Mat<2, 2, T> const& rhs);
		Mat<2, 2, T>& operator+=(T rhs);
		Mat<2, 2, T>& operator+=(Mat<2, 2, T> const& rhs);
		Mat<2, 2, T>& operator-=(T rhs);
		Mat<2, 2, T>& operator-=(Mat<2, 2, T> const& rhs);
		Mat<2, 2, T>& operator*=(T rhs);
		Mat<2, 2, T>& operator/=(T rhs);
		Mat<2, 2, T> operator-() const;
        
    //  Member functions
		ValueType GetCol(unsigned col) const;
		ValueType GetRow(unsigned row) const;
		void Transpose();
  };

  #include "Mat2.tpp"

  // Non-member operator overloads
  template <typename T>
  Mat<2, 2, T> operator+(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs);
  template <typename T>
  Mat<2, 2, T> operator+(Mat<2, 2, T> const& lhs, T rhs);
  template <typename T>
  Mat<2, 2, T> operator-(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs);
  template <typename T>
  Mat<2, 2, T> operator-(Mat<2, 2, T> const& lhs, T rhs);
  template <typename T>
  Mat<2, 2, T> operator*(T lhs, Mat<2, 2, T> const& rhs);
  template <typename T>
  Mat<2, 2, T> operator*(Mat<2, 2, T> const& lhs, T rhs);
  template <typename T>
	Vec<2, T> operator*(Mat<2, 2, T> const& lhs, Vec<2, T> const& rhs);
  template <typename T>
  Mat<2, 2, T> operator/(Mat<2, 2, T> const& lhs, T rhs);
  template <typename T>
  bool operator==(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs);
  template <typename T>
  bool operator!=(Mat<2, 2, T> const& lhs, Mat<2, 2, T> const& rhs);

} // namespace gpm

#endif