/*!*********************************************************************
\file   Vec2.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Specialized definition for Vector 3Ds from GEM library.
        Contains accessors, constructors, member and non-member
        operator overloads.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_VEC3_H
#define GOOP_VEC3_H

namespace GE
{
  namespace Math
  {
    template <typename T>
    struct Vec<3, T> {
      // ACCESSORS
      union { T x, r, s; };
      union { T y, g, t; };
      union { T z, b, p; };

      // CONSTRUCTORS
      Vec();
      Vec(T _x, T _y, T _z);
      Vec(Vec<3, T>&& rhs) = default;
      Vec(Vec<3, T> const& rhs);
      Vec(Vec<2, T> const& rhs);
      Vec(Vec<2, T> const& rhs, T _z);
      Vec(Vec<4, T> const& rhs);

      // OPERATOR OVERLOADS
      Vec<3, T>& operator=(Vec<3, T>&& rhs) = default;
      Vec<3, T>& operator=(Vec<3, T> const& rhs);
      Vec<3, T>& operator+=(Vec<3, T> const& rhs);
      Vec<3, T>& operator+=(T rhs);
      Vec<3, T>& operator-=(Vec<3, T> const& rhs);
      Vec<3, T>& operator-=(T rhs);
      Vec<3, T>& operator*=(T rhs);
      Vec<3, T>& operator/=(T rhs);
      Vec<3, T> operator-() const;
      T& operator[](size_type rhs);
      T const& operator[](size_type rhs) const;

      /*!*********************************************************************
      \brief
        Initializes a vector with values from a string
      \param rhs
       The string containing the vector's data in the form of { x, y, z }
      \return
        The vector object
      ************************************************************************/
      Vec<3, T>& operator<<(std::string const& rhs);

      //  MEMBER FUNCTIONS
      /*!***********************************************************************
      \brief
        Normalizes the current vector. Only supports floating-point types.
      *************************************************************************/
      void Normalize();

      /*!***********************************************************************
      \brief
        Converts the vector into an std::string in the form of { x, y, z }
      *************************************************************************/
      std::string ToString() const;
    };

    // NON-MEMBER OPERATOR OVERLOADS
    template <typename T>
    Vec<3, T> operator+(Vec<3, T> const& lhs, Vec<3, T> const& rhs);
    template <typename T>
    Vec<3, T> operator+(Vec<3, T> const& lhs, T rhs);
    template <typename T>
    Vec<3, T> operator+(T lhs, Vec<3, T> const& rhs);
    template <typename T>
    Vec<3, T> operator-(Vec<3, T> const& lhs, Vec<3, T> const& rhs);
    template <typename T>
    Vec<3, T> operator-(Vec<3, T> const& lhs, T rhs);
    template <typename T>
    Vec<3, T> operator-(Vec<3, T> const& lhs, T rhs);
    template <typename T>
    Vec<3, T> operator*(Vec<3, T> const& lhs, T rhs);
    template <typename T>
    Vec<3, T> operator*(T lhs, Vec<3, T> const& rhs);
    template <typename T>
    Vec<3, T> operator/(Vec<3, T> const& lhs, T rhs);
    template <typename T>
    bool operator==(Vec<3, T> const& lhs, Vec<3, T> const& rhs);
    template <typename T>
    bool operator!=(Vec<3, T> const& lhs, Vec<3, T> const& rhs);

#include "Vec3.tpp"
  }
} // namespace GE

#endif