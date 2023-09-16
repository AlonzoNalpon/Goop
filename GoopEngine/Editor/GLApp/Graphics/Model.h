#ifndef MODEL_H
#define MODEL_H
#include <Def.h>
// holds model data (opaque pointers)
struct Model {
  GLenum primitive_type; // which OpenGL primitive to be rendered?
  GLuint primitive_cnt;  // number of primitives
  GLuint vaoid;          // handle to VAO
  GLuint draw_cnt;       // vertex OR indices count
  GLuint shader;         // shader handle
};
#endif
