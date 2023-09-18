/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/10/2023

This is the basic sprite shader for rendering sprites

*//*__________________________________________________________________________*/
#version 460 core
layout (location=0) in vec3 aVertexPosition;  // vertex position attribute

layout (location=0) out vec3 vColor;    // vertex color output

// UNIFORMS GO HERE
void main() {
  gl_Position = vec4( aVertexPosition, 1.0);
  vColor      = aVertexColor;
}
