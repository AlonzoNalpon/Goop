#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H
#include <Def.h>
#include <type_traits>
#include <bit>
#include <vector>
#include <initializer_list>
#include <glm.hpp>
namespace Graphics {
// Minimize this region if you want to see the instances below templates
#pragma region TEMPLATES
  // Color template for 


#pragma warning(disable : 4201)   // anonymous structure in union
#pragma warning(disable : 26495)  // union member not initialized warning
  template <typename T, std::endian endian = std::endian::native>
  union T_Colorf;

  template <typename T>
  union T_Colorf <T, std::endian::big>{
    struct {
      T a, b, g, r;
    };

    T rgba[4];

    T_Colorf(T r = {}, T g = {}, T b = {}, T a = {}) :
      r{ r }, g{ g }, b{ b }, a{ a } {}
    
  };

  template <typename T>
  union T_Colorf <T, std::endian::little> {
    struct {
      T r, g, b, a;
    };

    T_Colorf(T r = {}, T g = {}, T b = {}, T a = {}) :
      r{ r }, g{ g }, b{ b }, a{ a } {}

    T rgba[4];
  };

  template <typename T>
  union T_Rect {
      struct {
        T width, height;
      };
      struct {
        T x, y;
      };

      T_Rect(T width = {}, T height = {}) :
        width{ width }, height{ height } {}
    };
#pragma warning(default : 26495) // union member not initialized warning
#pragma warning(default : 4201)  // anonymous structure in union

  // For SoA 
  template <typename Vector3, typename Color3, typename UV2, template <typename> class Cont>
  struct T_GL_Data_Layout {
    Cont<Vector3>   pos_vtx;
    Cont<Color3>    clr_vtx;
    Cont<UV2>       tex_vtx;
    T_GL_Data_Layout(std::initializer_list<Vector3> pos_data, 
                     std::initializer_list<Color3> clr_data, 
                     std::initializer_list<UV2> tex_data):
       pos_vtx{ pos_data }, clr_vtx{ clr_data }, tex_vtx{ tex_data }{}

    // Sizeof methods
    static constexpr GLsizeiptr PosTypeSize() {
      return sizeof(Vector3);
    }
    static constexpr GLsizeiptr ClrTypeSize() {
      return sizeof(Color3);
    }
    static constexpr GLsizeiptr TexTypeSize() {
      return sizeof(UV2);
    }

    // Container size methods
    GLsizeiptr PosDataSize() const
    {
      return static_cast<GLsizeiptr>(pos_vtx.size() * PosTypeSize());
    }
    GLsizeiptr ClrDataSize() const
    {
      return static_cast<GLsizeiptr>(clr_vtx.size() * ClrTypeSize());
    }
    GLsizeiptr TexDataSize() const
    {
      return static_cast<GLsizeiptr>(tex_vtx.size() * TexTypeSize());
    }
    // Helper to calculate total size of all attributes in bytes
    GLsizeiptr DataSize() const
    {
      return  PosDataSize() + ClrDataSize() + TexDataSize();
    }
  };

  template <typename Int, typename Float, typename Bool>
  struct T_GL_Material {
    Int shdrpgm;
    Bool enableLight;
    // All advanced effects here

  };

#pragma region TEMPORARY
  // TEMPORARY TYPES TO BE REPLACED
  template <typename T>
  struct T_Vec3 {
    T x, y, z;
    T_Vec3(T x = {}, T y = {}, T z = {}) : x{ x }, y{ y }, z{ z } {}
  };
  template <typename T>
  struct T_Vec2 {
    T x, y;
    T_Vec2(T x = 0, T y = 0) : x{ x }, y{ y } {}
  };
#pragma endregion // END TEMPORARY REGION
  using gVec3  = glm::vec3;   //!< glm vector3
  using gDvec3 = glm::dvec3;  //!< glm dvector3
  using gVec2  = glm::vec2;   //!< glm vector2
  using gDvec2 = glm::dvec2;  //!< glm dvector2
  using gIvec2 = glm::ivec2;  //!< glm ivector2

  using gObjID = size_t; //!< This is a type indicating ID of a specific object (not guaranteed to be identical to their OpenGL ID)
#pragma endregion // END TEMPLATES REGION
  using Colorf    = T_Colorf<f32>;
  using Rect      = T_Rect<GLint>;

  // GL_Data contains attribute data for transfer. Thus, it should use single precision floats
  using GL_Data   = T_GL_Data_Layout<gVec3, Colorf, gVec2, std::vector>;

  constexpr gObjID BAD_OBJ_ID{ static_cast<gObjID>(-1) };
}

#endif