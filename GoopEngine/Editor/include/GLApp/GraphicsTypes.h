#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H
#include <Def.h>
#include <type_traits>
#include <bit>
#include <vector>
#include <initializer_list>
namespace Graphics {

// Minimize this region if you want to see the instances below templates
#pragma region TEMPLATES
  // Color template for 
#pragma warning(disable : 4201)

  template <typename T, std::endian endian = std::endian::native>
  union T_Color;

  template <typename T>
  union T_Color <T, std::endian::big>{
    struct {
      T r, g, b, a;
    };
    T rgba[4];

    T_Color(T r = {}, T g = {}, T b = {}, T a = {}) :
      r{ r }, g{ g }, b{ b }, a{ a } {}
  };

  template <typename T>
  union T_Color <T, std::endian::little> {
    struct {
      T a, b, g, r;
    };
    T rgba[4];

    T_Color(T r = {}, T g = {}, T b = {}, T a = {}) :
      r{ r }, g{ g }, b{ b }, a{ a } {}
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

  // For SoA 
  template <typename Vector3, typename Color3, typename UV2, template <typename> class Cont>
  struct T_GL_Data_Layout {
    Cont<Vector3>  pos_vtx;
    Cont<Color3>  clr_vtx;
    Cont<UV2>   tex_vtx;
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
#pragma warning(default : 4201)

#pragma region TEMPORARY
  // TEMPORARY TYPES TO BE REPLACED
  template <typename T>
  struct T_Vec3 {
    T x, y, z;
    T_Vec3(T x = 0, T y = 0, T z = 0) : x{ x }, y{ y }, z{ z } {}
  };
  template <typename T>
  struct T_Vec2 {
    T x, y;
    T_Vec2(T x = 0, T y = 0) : x{ x }, y{ y } {}
  };
#pragma endregion // END TEMPORARY REGION
  using Vec3 = T_Vec3<f32>;
  using Dvec3 = T_Vec3<f64>;
  using Vec2 = T_Vec2<f32>;
  using Dvec2 = T_Vec2<f64>;
#pragma endregion // END TEMPLATES REGION
  using Color     = T_Color<f32>;
  using Rect      = T_Rect<GLint>;

  // GL_Data contains attribute data for transfer. Thus, it should use single precision floats
  using GL_Data   = T_GL_Data_Layout<Vec3, Color, Vec2, std::vector>;

  // holds model data (opaque pointers)
  struct Model {
    GLenum  primitive_type; // which OpenGL primitive to be rendered?
    GLuint  primitive_cnt;  // number of primitives
    GLuint  vaoid;          // handle to VAO
    GLuint  draw_cnt;       // vertex OR indices count
  };
}

#endif