
#version 460 core

layout (location=0) in vec3 aVertexPosition;  // vertex position attribute
layout (location=1) in vec3 aVertexColor;     // vertex color attribute
layout (location=2) in vec3 aVertexNormal;    // vertex normal attribute
layout (location=3) in vec2 aTexturePosition; // texture position attribute which we won't use

layout (location=0) out vec2 vTexCoord; // texture coordinate output

void main(){
  gl_Position = vec4(aVertexPosition.xy, 0.0, 1.0);

  vTexCoord = aVertexPosition.xy * 0.5 + 0.5; // mapping to [0,1]
}
