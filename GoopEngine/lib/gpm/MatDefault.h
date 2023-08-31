#ifndef GOOP_MAT_DEFAULT_H
#define GOOP_MAT_DEFAULT_H

namespace gpm
{
  template <unsigned Rows, unsigned Cols, typename T>
  struct Mat
  {
    // ensure only rows/cols of sizes 2, 3, 4 are instantiated
    static_assert(!(Rows < 2 || Rows > 4) || (Cols < 2 || Cols > 4), 
      "Mat<unsigned Row, unsigned Col, typename T>: only rows/cols of 2, 3 and 4 are supported!");
    using ValueType = Vec<Rows, T>;

    ValueType m_data[Cols];

    // Constructors
    Mat();
    Mat(Mat<Rows, Cols, T> const& rhs);

		// Accessors
		T& At(unsigned row, unsigned col);
		T const& At(unsigned row, unsigned col) const;
		ValueType& operator[](unsigned rhs);
		ValueType const& operator[](unsigned rhs) const;

    // Operator overloads
    Mat<Rows, Cols, T>& operator=(Mat<Rows, Cols, T> const& rhs);
		Mat<Rows, Cols, T>& operator+=(T rhs);
		Mat<Rows, Cols, T>& operator+=(Mat<Rows, Cols, T> const& rhs);
		Mat<Rows, Cols, T>& operator-=(T rhs);
		Mat<Rows, Cols, T>& operator-=(Mat<Rows, Cols, T> const& rhs);
		Mat<Rows, Cols, T>& operator*=(T rhs);
		Mat<Rows, Cols, T>& operator/=(T rhs);
		Mat<Rows, Cols, T> operator-() const;
        
    //  Member functions
		ValueType GetCol(unsigned col) const;
		Vec<Cols, T> GetRow(unsigned row) const;
		void Transpose();
  };

  #include "MatDefault.tpp"

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