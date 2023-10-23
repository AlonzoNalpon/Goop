/*!*********************************************************************
\file   GraphicsTypes.h
\author a.nalpon@digipen.edu
\date   28-September-2023
\brief  This file contains definitions and typedefs for types
        to be used with graphics engine
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
#pragma warning(disable : 4201)   // anonymous structure in union
#pragma warning(disable : 26495)  // union member not initialized warning
  /*!
   * \union T_Colorf
   * \brief  
   * implementation of color data type.
   * 4 values of template type representing red, green blue and alpha
   */
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

  /*!
   * union T_Rect
   * \brief  
   * Template representing rectangle
   * of width and height using data type template T.
   */
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


  /*!
   * \struct T_GL_Data_Layout
   * \brief  
   * the struct for vertex attribute arrays in SOA format.
   */
  template <typename Vector3, typename Color3, typename Normal3, typename UV2, template <typename> class Cont>
  struct T_GL_Data_Layout {
    using size_type = GLsizeiptr;
    Cont<Vector3>   pos_vtx;
    Cont<Color3>    clr_vtx;
    Cont<Normal3>   nml_vtx;
    Cont<UV2>       tex_vtx;
    T_GL_Data_Layout(std::initializer_list<Vector3> pos_data, 
                     std::initializer_list<Color3> clr_data, 
                     std::initializer_list<Normal3> nml_data,
                     std::initializer_list<UV2> tex_data):
      pos_vtx{ pos_data }, clr_vtx{ clr_data }, nml_vtx{nml_data}, tex_vtx { tex_data } 
    {}

    /*!*********************************************************************
    \brief
      Get the size of each position vertex.
    \params
    \return
      
    ************************************************************************/
    static constexpr size_type PosTypeSize() {
      return sizeof(Vector3);
    }
    /*!*********************************************************************
    \brief
      Get the size of each color vertex.
    \params
    \return
      
    ************************************************************************/
    static constexpr size_type ClrTypeSize() {
      return sizeof(Color3);
    }

    /*!*********************************************************************
    \brief
      Get the size of each normal vertex.
    \params
    \return
      
    ************************************************************************/
    static constexpr size_type NmlTypeSize() {
      return sizeof(Normal3);
    }

    /*!*********************************************************************
    \brief
      Get the size of each tex vertex.
    \params
    \return
      
    ************************************************************************/
    static constexpr size_type TexTypeSize() {
      return sizeof(UV2);
    }

    /*!*********************************************************************
    \brief
      Return total size of pos vertices in bytes.
    \params
    \return the total size of pos
      
    ************************************************************************/
    size_type PosDataSize() const
    {
      return static_cast<size_type>(pos_vtx.size() * PosTypeSize());
    }

    /*!*********************************************************************
    \brief
      Return the total size of clr vertices in bytes.
    \params
    \return the total size of clr
      
    ************************************************************************/
    size_type ClrDataSize() const
    {
      return static_cast<size_type>(clr_vtx.size() * ClrTypeSize());
    }

    /*!*********************************************************************
    \brief
      Return the total size of nml vertices in bytes.
    \params
    \return
      
    ************************************************************************/
    size_type NmlDataSize() const
    {
      return static_cast<size_type>(nml_vtx.size() * NmlTypeSize());
    }

    /*!*********************************************************************
    \brief
      Return the total size of tex verts in bytes.
    \params
    \return the total size of tex
      
    ************************************************************************/
    size_type TexDataSize() const
    {
      return static_cast<size_type>(tex_vtx.size() * TexTypeSize());
    }
    
    /*!*********************************************************************
    \brief
      Return the total size of all attributes combined in bytes.
    \params
    \return the total size of all attributes
      
    ************************************************************************/
    size_type DataSize() const
    {
      return  PosDataSize() + ClrDataSize() + NmlDataSize() + TexDataSize();
    }
  };

  template <typename T>
  struct T_DeferredShdrs {
    T gPassShader;
    T dPassShader;
  };

  /*!
   * \struct T_GL_Material
   * \brief  
   * An unused struct for more advanced feats, if time permits.
   */
  template <typename Int, typename Float, typename Bool>
  struct T_GL_Material {
    Int shdrpgm;
    Bool enableLight;
    // All advanced effects here

  };

  using gVec3  = glm::vec3;   //!< glm vector3
  using gDvec3 = glm::dvec3;  //!< glm dvector3
  using gVec2  = glm::vec2;   //!< glm vector2
  using gDvec2 = glm::dvec2;  //!< glm dvector2
  using gIvec2 = glm::ivec2;  //!< glm ivector2

  using gObjID      = size_t; //!< This is a type indicating ID of a specific object (not guaranteed to be identical to their OpenGL ID)
#pragma endregion // END TEMPLATES REGION
  using Colorf        = T_Colorf<f32>; //!< color structure for single-precision floating point rgba values
  using Rect          = T_Rect<GLint>; //!< rect structure to be measured in pixels

  // GL_Data contains attribute data for transfer. Thus, it should use single precision floats
  // position, color, texcoordinates
  using GL_Data       = T_GL_Data_Layout<gVec3, Colorf, gVec3, gVec2, std::vector>; //!< our most common GL data type for shader attributes

  // For deferred rendering, multiple passes are required. The type for the shader program handles would be GLuint
  using Render_Shdrs = T_DeferredShdrs<GLuint>;
  
  /*!
   * \struct  GBufferAtt
   * \brief  
   * Handles to G buffer attachments.
   */
  struct DeferredInfo {
    GLuint posTexture{};
    GLuint normalTexture{};
    GLuint albedoTexture{};
    GLuint gBuffer{};
    GLuint finalBuffer{};
  };

  constexpr gObjID BAD_OBJ_ID{ static_cast<gObjID>(-1) }; // Our bad obj id will be the biggest number possible
}

#endif