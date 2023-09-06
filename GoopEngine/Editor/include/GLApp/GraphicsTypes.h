#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H
#include <def.h>
namespace Graphics {

// Minimize this region if you want to see the instances below templates
#pragma region TEMPLATES
  // Color template for 
  template <typename T>
  union T_Color {
    struct {
      T r, g, b, a;
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

  template <typename Vec3, typename Clr3, typename UV2, template <typename> class Cont>
  struct T_GL_Data_Layout {
    Cont<Vec3>  pos_vtx;
    Cont<Clr3>  clr_vtx;
    Cont<UV2>   tex_vtx;
  };


#pragma endregion
  
  using Color     = T_Color<f32>;
  using Rect      = T_Rect<GLint>;
  //using GL_Data   = T_GL_Data_Layout<>;
}

#endif