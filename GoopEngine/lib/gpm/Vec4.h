#ifndef GOOP_VEC4_H
#define GOOP_VEC4_H

namespace gpm
{
  template <typename T>
  struct Vec<4, T> {
    union { T x, r, s; };
	  union { T y, g, t; };
	  union { T z, b, p; };
    union { T w, a, q; };
    // Constructors
    Vec();
    Vec(T _x, T _y, T _z, T _w);
    Vec(Vec<4, T> const& rhs);
    Vec(Vec<2, T> const& rhs);
    Vec(Vec<3, T> const& rhs);
    
    // Operator overloads
    Vec<4, T>& operator=(Vec<4, T> const& rhs);
    Vec<4, T>& operator+=(Vec<4, T> const& rhs);
    Vec<4, T>& operator+=(T rhs);
    Vec<4, T>& operator-=(Vec<4, T> const& rhs);
    Vec<4, T>& operator-=(T rhs);
    Vec<4, T>& operator*=(T rhs);
    Vec<4, T>& operator/=(T rhs);
    T& operator[](unsigned rhs);
    T const& operator[](unsigned rhs) const;
    
    //  Member functions
    void Normalize();
  };
  
  // Non-member operator overloads
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
  Vec<4, T> operator-(Vec<4, T> const& rhs);
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

  #include "Vec4.tpp"
} // namespace gpm

#endif