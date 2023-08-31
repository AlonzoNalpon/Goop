#ifndef GOOP_VEC3_H
#define GOOP_VEC3_H

namespace gpm 
{
  template <typename T>
  struct Vec<3, T> {
    union { T x, r, s; };
	  union { T y, g, t; };
	  union { T z, b, p; };

    // Constructors
    Vec();
    Vec(T _x, T _y, T _z);
    Vec(Vec<3, T> const& rhs);
    Vec(Vec<2, T> const& rhs);
    Vec(Vec<4, T> const& rhs);
    
    // Operator overloads
    Vec<3, T>& operator=(Vec<3, T> const& rhs);
    Vec<3, T>& operator+=(Vec<3, T> const& rhs);
    Vec<3, T>& operator+=(T rhs);
    Vec<3, T>& operator-=(Vec<3, T> const& rhs);
    Vec<3, T>& operator-=(T rhs);
    Vec<3, T>& operator*=(T rhs);
    Vec<3, T>& operator/=(T rhs);
    T& operator[](unsigned rhs);
    T const& operator[](unsigned rhs) const;
    
    //  Member functions
    void Normalize();
  };
  
  // Non-member operator overloads
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
} // namespace gpm

#endif