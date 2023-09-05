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

void MtxIdentity(Mat3& mtx)
{
  mtx[0] = Mat3::ValueType(1.f, 0.f, 0.f);
  mtx[1] = Mat3::ValueType(0.f, 1.f, 0.f);
  mtx[2] = Mat3::ValueType(0.f, 0.f, 1.f);
}

void MtxScale(Mat3& mtx, float x, float y)
{
  mtx[0] = Mat3::ValueType(x, 0.f, 0.f);
  mtx[1] = Mat3::ValueType(0.f, y, 0.f);
  mtx[2] = Mat3::ValueType(0.f, 0.f, 1.f);
}

void MtxRotate(Mat3& mtx, float radians)
{
  const float cosRad{ cosf(radians) }, sinRad{ sinf(radians) };
  mtx[0] = Mat3::ValueType(cosRad, -sinRad, 0.f);
  mtx[1] = Mat3::ValueType(sinRad, cosRad, 0.f);
  mtx[2] = Mat3::ValueType(0.f, 0.f, 1.f);
}

void MtxTranslate(Mat3& mtx, float x, float y)
{
  mtx[0] = Mat3::ValueType(0.f, 0.f, x);
  mtx[1] = Mat3::ValueType(0.f, 0.f, y);
  mtx[2] = Mat3::ValueType(0.f, 0.f, 1.f);
}

void MtxInverse(Mat3& result, Mat3 const& mtx)
{
  float const det { MtxDeterminant(mtx) };
  if (det == 0.f) { throw std::runtime_error("division by zero"); }

  float const inverseDet{ 1.f / det };
  Mat3 const temp{ mtx };
  result[0].x = (temp[1].y * temp[2].z - temp[1].z * temp[2].y) * inverseDet;
	result[1].x = (temp[2].x * temp[1].z - temp[1].x * temp[2].z) * inverseDet;
	result[2].x = (temp[1].x * temp[2].y - temp[2].x * temp[1].y) * inverseDet;
	result[0].y = (temp[2].y * temp[0].z - temp[0].y * temp[2].z) * inverseDet;
	result[1].y = (temp[0].x * temp[2].z - temp[2].x * temp[0].z) * inverseDet;
	result[2].y = (temp[0].y * temp[2].x - temp[0].x * temp[2].y) * inverseDet;
	result[0].z = (temp[0].y * temp[1].z - temp[0].z * temp[1].y) * inverseDet;
	result[1].z = (temp[0].z * temp[1].x - temp[0].x * temp[1].z) * inverseDet;
	result[2].z = (temp[0].x * temp[1].y - temp[0].y * temp[1].x) * inverseDet;
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