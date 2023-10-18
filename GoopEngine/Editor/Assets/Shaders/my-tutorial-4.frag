/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/01/2023

This file is a basic fragment shader meant to work with my-tutorial-3.vert
shader.

tutorial 4: a per-fragment color attribute is no longer received from rasterizer.
Instead, per-fragment color attribute is supplied by the application to
a uniform variable: uniform vec3 uColor;

This uniform variblae will retain the value for every invocation of the
fragment shader. That is why every fragment of the triangle primitive
rendered by an object has the same color.
*//*__________________________________________________________________________*/
#version 460 core

// layout (location=0) in vec3 vInterpColor; deprecated
uniform vec3 uColor;

layout (location=0) out vec4 fFragColor;

void main(void){
    //fFragColor = vec4(vInterpColor, 1.0);
    fFragColor = vec4(uColor, 1.0);
}