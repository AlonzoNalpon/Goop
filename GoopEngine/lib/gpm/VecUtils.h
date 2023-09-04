#ifndef GOOP_VEC_UTILS_H
#define GOOP_VEC_UTILS_H

namespace gpm
{
  /*!***********************************************************************
	\brief
		Returns the normalized form of a gpm::Vec
  \param[in] vector
    The vector to normalize
  \return
    The normalized vector
	*************************************************************************/
  template<typename T>

  Vec<2, T> Normalize(Vec<2, T> const& vector);
  /*!***********************************************************************
	\brief
		Returns the normalized form of a gpm::Vec
  \param[in] vector
    The vector to normalize
  \return
    The normalized vector
	*************************************************************************/
  template<typename T>

  Vec<3, T> Normalize(Vec<3, T> const& vector);
  /*!***********************************************************************
	\brief
		Returns the normalized form of a gpm::Vec
  \param[in] vector
    The vector to normalize
  \return
    The normalized vector
	*************************************************************************/
  template<typename T>
  Vec<4, T> Normalize(Vec<4, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the length of a gpm::Vec
  \param[in] vector
    The vector to compute the length of
  \return
    The length of the vector
	*************************************************************************/
  template<typename T>
  T GetMagnitude(Vec<2, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the length of a gpm::Vec
  \param[in] vector
    The vector to compute the length of
  \return
    The length of the vector
	*************************************************************************/
  template<typename T>
  T GetMagnitude(Vec<3, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the length of a gpm::Vec
  \param[in] vector
    The vector to compute the length of
  \return
    The length of the vector
	*************************************************************************/
  template<typename T>
  T GetMagnitude(Vec<4, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the squared length of a gpm::Vec. Use to avoid the square root
    function.
  \param[in] vector
    The vector to compute the squared length of
  \return
    The length of the vector squared
	*************************************************************************/
  template<typename T>
  T GetSquaredMagnitude(Vec<2, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the squared length of a gpm::Vec. Use to avoid the square root
    function.
  \param[in] vector
    The vector to compute the squared length of
  \return
    The length of the vector squared
	*************************************************************************/
  template<typename T>
  T GetSquaredMagnitude(Vec<3, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the squared length of a gpm::Vec. Use to avoid the square root
    function.
  \param[in] vector
    The vector to compute the squared length of
  \return
    The length of the vector squared
	*************************************************************************/
  template<typename T>
  T GetSquaredMagnitude(Vec<4, T> const& vector);

  /*!***********************************************************************
	\brief
		Returns the dot product of 2 vectors
  \param[in] lhs
    The lhs of the operation
  \param[in] rhs
    The rhs of the operation
  \return
    The value of the dot product
	*************************************************************************/
  template<typename T>
  T Dot(Vec<2, T> const& lhs, Vec<2, T> const& rhs);

  /*!***********************************************************************
	\brief
		Returns the dot product of 2 vectors
  \param[in] lhs
    The lhs of the operation
  \param[in] rhs
    The rhs of the operation
  \return
    The value of the dot product
	*************************************************************************/
  template<typename T>
  T Dot(Vec<3, T> const& lhs, Vec<3, T> const& rhs);

  /*!***********************************************************************
	\brief
		Returns the dot product of 2 vectors
  \param[in] lhs
    The lhs of the operation
  \param[in] rhs
    The rhs of the operation
  \return
    The value of the dot product
	*************************************************************************/
  template<typename T>
  T Dot(Vec<4, T> const& lhs, Vec<4, T> const& rhs);

  /*!***********************************************************************
	\brief
		Returns the cross product of 2 vectors
  \param[in] lhs
    The lhs of the operation
  \param[in] rhs
    The rhs of the operation
  \return
    The value of the cross product
	*************************************************************************/
  template<typename T>
  Vec<3, T> Cross(Vec<3, T> const& lhs, Vec<3, T> const& rhs);

  #include "VecUtils.tpp"

} // namespace gpm

#endif