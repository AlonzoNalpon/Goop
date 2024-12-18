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
  T det{};
  Mat<3, 3, T> subMtx{};
  for (size_type i{}; i < 4; ++i)
  {
    for (size_type j{}; j < 3; ++j)
    {
      size_type col{};
      for (size_type k = 0; k < 4; ++k)
      {
        if (k == i) { continue; }

        subMtx[j][col] = mtx[j + 1][k];
        ++col;
      }
    }
     det += mtx[0][i] * MtxDeterminant(subMtx) * (i % 2 == 0 ? 1 : -1);
  }
  return det;
}