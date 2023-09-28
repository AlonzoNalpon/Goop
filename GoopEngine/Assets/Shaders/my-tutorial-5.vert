/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/10/2023

Basic vertex shader meant to work with my-tutorial-5.frag
shader.
As of tutorial 5, texture mapping is the main focus. Multiple demos
split from different tasks use this shader.

*//*__________________________________________________________________________*/
#version 450 core
layout (location=0) in vec2 aVertexPosition;  // vertex position attribute
layout (location=1) in vec3 aVertexColor;     // vertex color attribute
layout (location=2) in vec2 aTexturePosition; // texture position attribute

layout (location=0) out vec3 vColor;    // vertex color output
layout (location=1) out vec2 vTexCoord; // texture coordinate output

uniform int uTask_ID;
void main() {
  gl_Position = vec4( aVertexPosition, 0.0, 1.0);
  vColor      = aVertexColor;
  vTexCoord   = aTexturePosition;
  switch(uTask_ID){
    case 4: // repeat
    case 5: // mirror
    case 6: // clamp
      vTexCoord *= 4;
      break;
    default:
      break;
  }
}