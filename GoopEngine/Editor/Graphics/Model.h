#ifndef MODEL_H
#define MODEL_H
#include <Def.h>
/*!
 * \struct Model
 * \brief  
 * This struct contains information required for rendering objects.
 * 
 */
namespace Graphics
{
  struct Model {
    GLenum primitive_type; //!<which OpenGL primitive to be rendered? (triangle strip for example)
    GLuint primitive_cnt;  //!<number of primitives 
    GLuint vaoid;          //!<handle to VAO
    GLuint draw_cnt;       //!<vertex OR indices count
    GLuint shader;         //!<shader handle
  };
}
#endif
