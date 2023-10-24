template <size_type N, size_type M, size_type P, typename T>
Mat<N, P, T> operator*(Mat<N, M, T> const& lhs, Mat<M, P, T> const& rhs)
{
  Mat<N, P, T> temp{};
  for (size_type i{}; i < N; ++i)
  {
    for (size_type j{}; j < P; ++j)
    {
      temp[i][j] = Dot(lhs[i], rhs.GetCol(j));
    }
  }

  return temp;
}

template <size_type N, size_type M, typename T>
void MtxTranspose(Mat<N, M, T>& result, Mat<M, N, T> const& mtx)
{
  Mat<M, N, T> const temp{ mtx };
  for (size_type i{}; i < M; ++i)
  {
    result[i] = temp.GetCol(i);
  }
}

template <typename T>
T MtxDeterminant(Mat<3, 3, T> const& mtx)
{
  return (mtx[0].x * mtx[1].y * mtx[2].z + mtx[0].y * mtx[1].z * mtx[2].x
			+ mtx[0].z * mtx[1].x * mtx[2].y) - (mtx[0].z * mtx[1].y * mtx[2].x +
				mtx[0].y * mtx[1].x * mtx[2].z + mtx[0].x * mtx[1].z * mtx[2].y);
}

template <typename T>
T MtxDeterminant(Mat<4, 4, T> const& mtx)
{
  T det = 0;

  for (int i = 0; i < 4; ++i) {
    // Create a submatrix by removing the i-th row and 0-th column
    Mat<3, 3, T> submatrix;
    for (int row = 0; row < 3; ++row) {
      for (int col = 1; col < 4; ++col) {
        submatrix[row][col - 1] = mtx[row][col];
      }
    }

    // Calculate the cofactor for this element and recursively find the determinant of the submatrix
    T cofactor = (i % 2 == 0) ? mtx[0][i] : -mtx[0][i];
    T submatrix_det = MtxDeterminant(submatrix);

    // Add the contribution of this element to the determinant
    det += cofactor * submatrix_det;
  }

  return det;
}