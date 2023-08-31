#ifndef GOOP_VEC2_H
#define GOOP_VEC2_H

namespace gpm
{
  template <typename T>
  struct Vec<2, T> {
    union { T x, r, s; };
    union { T y, g, t; };

    // Constructors
    Vec();
    Vec(T _x, T _y);
    Vec(Vec<2, T> const& rhs);
    Vec(Vec<3, T> const& rhs);
    Vec(Vec<4, T> const& rhs);

    // Operator overloads
    Vec<2, T>& operator=(Vec<2, T> const& rhs);
    Vec<2, T>& operator+=(Vec<2, T> const& rhs);
    Vec<2, T>& operator+=(T rhs);
    Vec<2, T>& operator-=(Vec<2, T> const& rhs);
    Vec<2, T>& operator-=(T rhs);
    Vec<2, T>& operator*=(T rhs);
    Vec<2, T>& operator/=(T rhs);
    Vec<2, T> operator-() const;
    T& operator[](unsigned rhs);
    T const& operator[](unsigned rhs) const;

    //  Member functions
    void Normalize();
  };

  // Non-member operator overloads
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
} // namespace gpm

#endif