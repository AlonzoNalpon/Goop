/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/10/2023

This is the basic sprite shader for rendering sprites

*//*__________________________________________________________________________*/
#version 460 core

layout (location=0) in vec3 aVertexPosition;  // vertex position attribute
layout (location=1) in vec3 aVertexColor;     // vertex color attribute

layout (location=0) uniform vec4 uEndPts;  // endpoints as a vec4 (pair of vec 2)
layout (location=1) uniform vec3 uColor;   // color of the line 
layout (location=0) out vec3 vColor;    // vertex color output

// UNIFORMS GO HERE
void main() {
  vec2 position = mix(vec2(uEndPts.x, uEndPts.y), vec2(uEndPts.z, uEndPts.w), gl_VertexID / 1.0);
  gl_Position = vec4(position, 0.0, 1.0);
  //gl_Position = vec4(aVertexPosition, 1.0);

  vColor        = uColor;
}
