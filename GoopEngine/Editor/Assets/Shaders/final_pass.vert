
#version 460 core
layout (location=0) in vec3 aVertexPosition;  // vertex position attribute
layout (location=1) in vec3 aVertexColor;     // vertex color attribute
layout (location=2) in vec2 aTexturePosition; // texture position attribute

layout (location=0) out vec2 vTexCoord; // texture coordinate output

// UNIFORMS GO HERE
void main() {
  gl_Position = vec4(aVertexPosition, 1.0);
  vTexCoord   = aTexturePosition;
}
