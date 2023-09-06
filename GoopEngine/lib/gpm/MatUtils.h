#ifndef GOOP_MAT_UTILS_H
#define GOOP_MAT_UTILS_H

namespace GPM
{
  template <size_type N, size_type M, size_type P, typename T>
  Mat<N, P, T> operator*(Mat<N, M, T> const& lhs, Mat<M, P, T> const& rhs);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat3 and turns it into an identity matrix
	\param[in] mtx
		The matrix to store the result
	*************************************************************************/
  void MtxIdentity(Mat3& mtx);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and turns it into an identity matrix
	\param[in] mtx
		The matrix to store the result
	*************************************************************************/
  void MtxIdentity(dMat3& mtx);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat3 and sets it to a scale matrix of x and y
	\param[in] mtx
		The matrix to store the result
  \param[in] x
    The x-component of the scale
  \param[in] y
    The y-component of the scale
	*************************************************************************/
  void MtxScale(Mat3& mtx, float x, float y);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and sets it to a scale matrix of x and y
	\param[in] mtx
		The matrix to store the result
  \param[in] x
    The x-component of the scale
  \param[in] y
    The y-component of the scale
	*************************************************************************/
  void MtxScale(dMat3& mtx, float x, float y);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat3 and sets it to a rotation matrix of the given 
    angle (counter-clockwise)
	\param[in] mtx
		The matrix to store the result
  \param[in] radians
    The angle in radians
	*************************************************************************/
  void MtxRotate(Mat3& mtx, float radians);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and sets it to a rotation matrix of the given 
    angle (counter-clockwise)
	\param[in] mtx
		The matrix to store the result
  \param[in] radians
    The angle in radians
	*************************************************************************/
  void MtxRotate(dMat3& mtx, float radians);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat3 and sets it to a translation matrix of x and y
	\param[in] mtx
		The matrix to store the result
  \param[in] x
    The x-component of the translate
  \param[in] y
    The y-component of the translate
	*************************************************************************/
  void MtxTranslate(Mat3& mtx, float x, float y);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and sets it to a translation matrix of x and y
	\param[in] mtx
		The matrix to store the result
  \param[in] x
    The x-component of the translate
  \param[in] y
    The y-component of the translate
	*************************************************************************/
  void MtxTranslate(dMat3& mtx, float x, float y);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and sets the result to the inverse of mtx
	\param[in] result
		The matrix to store the result
  \param[in] mtx
    The matrix to inverse
	*************************************************************************/
  void MtxInverse(Mat3& result, dMat3 const& mtx);

  /*!***********************************************************************
	\brief
		Takes in a gpm::dMat3 and sets the result to the inverse of mtx
	\param[in] result
		The matrix to store the result
  \param[in] mtx
    The matrix to inverse
	*************************************************************************/
  void MtxInverse(dMat3& result, dMat3 const& mtx);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat and sets the result to the transpose
	\param[in] result
		The matrix to store the result
  \param[in] mtx
    The matrix to inverse
	*************************************************************************/
  template <size_type N, size_type M, typename T>
  void MtxTranspose(Mat<N, M, T>& result, Mat<M, N, T> const& mtx);

  /*!***********************************************************************
	\brief
		Takes in a gpm::Mat3 returns its determinant
  \param[in] mtx
    The input matrix
	*************************************************************************/
  template <typename T>
  T MtxDeterminant(Mat<3, 3, T> const& mtx);
  
  #include "MatUtils.tpp"

} // namespace GPM

#endif