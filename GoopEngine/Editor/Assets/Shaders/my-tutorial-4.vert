/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/01/2023

Basic vertex shader meant to work with my-tutorial-3.frag
shader.
As of tutorial 4, a camera transform matrix has been implemented on the cpu
and is passed into the gpu via the uModel_to_NDC uniform variable

*//*__________________________________________________________________________*/
#version 450 core
layout (location=0) in vec2 aVertexPosition;

// tutorial 4: obj will have 1 color stored 
//layout (location=1) in vec3 aVertexColor;  // deprecated
//layout (location=0) out vec3 vColor;       // deprecated

uniform mat3 uModel_to_NDC;

void main() {
  gl_Position = vec4( vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)),
     0.0, 1.0);
  //vColor      = aVertexColor; // deprecated
}