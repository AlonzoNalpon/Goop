/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/10/2023

This is the basic sprite shader for rendering sprites
*//*__________________________________________________________________________*/
#version 460 core

layout (location=0) in vec3 vInterpColor;   // interpolated color
layout (location=1) in vec2 vTexCoord;      // 

layout (location=0) out vec4 fFragClr;      // output fragment color

uniform sampler2D uTex2d; // For task 3: struct to access image

void main(void){

    fFragClr = vec4(vInterpColor, 1.0); // by default: use object's interpolated color
}