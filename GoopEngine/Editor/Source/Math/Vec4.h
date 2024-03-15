/*!*********************************************************************
\file   Vec2.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Specialized definition for Vector 4Ds from GEM library.
        Contains accessors, constructors, member and non-member
        operator overloads.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GOOP_VEC4_H
#define GOOP_VEC4_H

namespace GE
{
  namespace Math
  {
    template <typename T>
    struct Vec<4, T> {
      // ACCESSORS
      union { T x, r, s; };
      union { T y, g, t; };
      union { T z, b, p; };
      union { T w, a, q; };

      // CONSTRUCTORS
      Vec();
      Vec(T _x, T _y, T _z, T _w);
      Vec(Vec<4, T>&& rhs) = default;
      Vec(Vec<4, T> const& rhs);
      Vec(Vec<2, T> const& rhs);
      Vec(Vec<2, T> const& rhs, T _z, T _w);
      Vec(Vec<3, T> const& rhs);
      Vec(Vec<3, T> const& rhs, T _w);

      template <typename S>
      Vec(Vec<4, S> const& rhs);
      template <typename S>
      Vec(Vec<2, S> const& rhs);
      template <typename S>
      Vec(Vec<2, S> const& rhs, T _z, T _w);
      template <typename S>
      Vec(Vec<3, S> const& rhs);
      template <typename S>
      Vec(Vec<3, S> const& rhs, T _w);

      // OPERATOR OVERLOADS
      Vec<4, T>& operator=(Vec<4, T>&& rhs) = default;
      Vec<4, T>& operator=(Vec<4, T> const& rhs);
      Vec<4, T>& operator+=(Vec<4, T> const& rhs);
      Vec<4, T>& operator-=(Vec<4, T> const& rhs);
      Vec<4, T>& operator+=(T rhs);
      Vec<4, T>& operator-=(T rhs);
      Vec<4, T>& operator*=(T rhs);
      Vec<4, T>& operator/=(T rhs);

      template <typename S>
      Vec<4, T>& operator+=(S rhs);
      template <typename S>
      Vec<4, T>& operator-=(S rhs);
      template <typename S>
      Vec<4, T>& operator*=(S rhs);
      template <typename S>
      Vec<4, T>& operator/=(S rhs);

      Vec<4, T> operator-() const;
      T& operator[](size_type rhs);
      T const& operator[](size_type rhs) const;

      /*!*********************************************************************
      \brief
        Initializes a vector with values from a string
      \param rhs
       The string containing the vector's data in the form of { x, y, z, w }
      \return
        The vector object
      ************************************************************************/
      Vec<4, T>& operator<<(std::string const& rhs);

      //  MEMBER FUNCTIONS
      /*!***********************************************************************
      \brief
        Normalizes the current vector. Only supports floating-point types.
      *************************************************************************/
      void Normalize();

      /*!***********************************************************************
      \brief
        Converts the vector into an std::string in the form of { x, y, z, w }
      *************************************************************************/
      std::string ToString() const;
    };

    // NON-MEMBER OPERATOR OVERLOADS
    template <typename T>
    Vec<4, T> operator+(Vec<4, T> const& lhs, Vec<4, T> const& rhs);
    template <typename T>
    Vec<4, T> operator+(Vec<4, T> const& lhs, T rhs);
    template <typename T>
    Vec<4, T> operator+(T lhs, Vec<4, T> const& rhs);
    template <typename T>
    Vec<4, T> operator-(Vec<4, T> const& lhs, Vec<4, T> const& rhs);
    template <typename T>
    Vec<4, T> operator-(Vec<4, T> const& lhs, T rhs);
    template <typename T>
    Vec<4, T> operator*(Vec<4, T> const& lhs, T rhs);
    template <typename T>
    Vec<4, T> operator*(T lhs, Vec<4, T> const& rhs);
    template <typename T>
    Vec<4, T> operator/(Vec<4, T> const& lhs, T rhs);
    template <typename T>
    bool operator==(Vec<4, T> const& lhs, Vec<4, T> const& rhs);
    template <typename T>
    bool operator!=(Vec<4, T> const& lhs, Vec<4, T> const& rhs);

    template <typename T, typename S>
    Vec<4, T> operator+(S lhs, Vec<4, T> const& rhs);
    template <typename T, typename S>
    Vec<4, T> operator+(Vec<4, T> const& lhs, S rhs);
    template <typename T, typename S>
    Vec<4, T> operator-(Vec<4, T> const& lhs, S rhs);
    template <typename T, typename S>
    Vec<4, T> operator*(S lhs, Vec<4, T> const& rhs);
    template <typename T, typename S>
    Vec<4, T> operator*(Vec<4, T> const& lhs, S rhs);
    template <typename T, typename S>
    Vec<4, T> operator/(Vec<4, T> const& lhs, S rhs);

#include "Vec4.tpp"
  }
} // namespace GE

#endif