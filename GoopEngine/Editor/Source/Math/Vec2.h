/*!*********************************************************************
\file   Vec2.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Specialized definition for Vector 2Ds from GEM library.
				Contains accessors, constructors, member and non-member
				operator overloads.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_VEC2_H
#define GOOP_VEC2_H

namespace GE
{
  namespace Math
  {
    template <typename T>
    struct Vec<2, T> {
      // ACCESSORS
      union { T x, r, s; };
      union { T y, g, t; };

      // CONSTRUCTORS
      Vec();
      Vec(T _x, T _y);
      Vec(Vec<2, T>&& rhs) = default;
      Vec(Vec<2, T> const& rhs);
      Vec(Vec<3, T> const& rhs);
      Vec(Vec<4, T> const& rhs);

      // OPERATOR OVERLOADS
      Vec<2, T>& operator=(Vec<2, T>&& rhs) = default;
      Vec<2, T>& operator=(Vec<2, T> const& rhs);
      Vec<2, T>& operator+=(Vec<2, T> const& rhs);
      Vec<2, T>& operator+=(T rhs);
      Vec<2, T>& operator-=(Vec<2, T> const& rhs);
      Vec<2, T>& operator-=(T rhs);
      Vec<2, T>& operator*=(T rhs);
      Vec<2, T>& operator/=(T rhs);
      Vec<2, T> operator-() const;
      T& operator[](size_type rhs);
      T const& operator[](size_type rhs) const;

      /*!*********************************************************************
      \brief
        Initializes a vector with values from a string
      \param rhs
       The string containing the vector's data in the form of { x, y }
      \return
        The vector object
      ************************************************************************/
      Vec<2, T>& operator<<(std::string const& rhs);

      //  MEMBER FUNCTIONS
      /*!***********************************************************************
      \brief
        Normalizes the current vector. Only supports floating-point types.
      *************************************************************************/
      void Normalize();

      /*!***********************************************************************
      \brief
        Converts the vector into an std::string in the form of { x, y }
      *************************************************************************/
      std::string ToString() const;
    };

    // NON-MEMBER OPERATOR OVERLOADS
    template <typename T>
    Vec<2, T> operator+(Vec<2, T> const& lhs, Vec<2, T> const& rhs);
    template <typename T>
    Vec<2, T> operator+(Vec<2, T> const& lhs, T rhs);
    template <typename T>
    Vec<2, T> operator+(T lhs, Vec<2, T> const& rhs);
    template <typename T>
    Vec<2, T> operator-(Vec<2, T> const& lhs, Vec<2, T> const& rhs);
    template <typename T>
    Vec<2, T> operator-(Vec<2, T> const& lhs, T rhs);
    template <typename T>
    Vec<2, T> operator*(Vec<2, T> const& lhs, T rhs);
    template <typename T>
    Vec<2, T> operator*(T lhs, Vec<2, T> const& rhs);
    template <typename T>
    Vec<2, T> operator/(Vec<2, T> const& lhs, T rhs);
    template <typename T>
    bool operator==(Vec<2, T> const& lhs, Vec<2, T> const& rhs);
    template <typename T>
    bool operator!=(Vec<2, T> const& lhs, Vec<2, T> const& rhs);

#include "Vec2.tpp"
  }
} // namespace GE

#endif