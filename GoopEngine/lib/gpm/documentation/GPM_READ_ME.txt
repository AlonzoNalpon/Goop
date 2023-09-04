/******** GOOP MATH LIBRARY (GPM) ********/

U can check out VectorUtils.html, MatrixUtils.html shortcuts in this folder
to view the current list of functions
and Vec2Struct.html, Mat3Struct.html to see the struct layout for Vecs and Mats

BASIC USAGE
-----------------------------
#include "gpm.h" to use

Everything is under namespace "gpm", so prefix "gpm::" before any functions/objects
 - E.g. gpm::Vec2, gpm::Vec3 etc.

Note: Only vec2s, 3s and 4s are supported.
Any vector/matrix instantiated with > 4 rows/cols will yield an error from static_assert()

For float-typed vectors and matrices u may use the following typedefs:
 - gpm::Vec2, gpm::Vec3, gpm::Vec4
 - gpm::Mat2, gpm::Mat3, gpm::Mat4

Similar to alpha engine, the following functions are available
 - MtxScale, MtxRotate, MtxTranslate
 - Cross, Dot
 - full list can be found in VectorUtils.html, MatrixUtils.html

Vectors and Matrices have operators overloaded to support most operations,
including multiplication
To concatenate / multiply matrices/vectors, simply do:
 - lhs * rhs

Components of vectors can be accessed through
 - (.x, .y, .z, .w),
 - (.r, .g, .b, .a), or
 - (.s, .t, .p, .q)
 - subscript operator []

More on Matrices
-----------------------------
Matrices are defined by an array of Vectors
 - e.g. A 3x3 matrix is made up of an array of 3 Vec3s

    So doing subscript operator [] on a matrix will return
    a reference to its udnerlying vector
     - e.g. for a Mat3 mat, mat[0] returns the first
            col in the form of a vec3

Other ways of accessing elements include member
functions .At() and .GetRow(), .GetCol()


ADVANCED USAGE
-----------------------------
To instantiate a vector of another type, do:
gpm::Vec<size, type>
 - e.g. Vec<3, double> creates a vec3 of type double

    Note that non-floating-point types will not be able to
    do any square root-related functions such as .Normalize().
    An error will be given by static_assert().

To instantiate a matrix of varying size or type, do:
gpm::Mat<rows, cols, type>
 - e.g. Mat<4, 2, double> creates a 4x2 matrix of type double

    Stuff like multiplication and non-member Transpose() function 
    will still work with these matrices