
#version 460 core

layout (location=0) in vec3 vInterpColor;   // interpolated color

layout (location=0) out vec4 fFragClr;      // output fragment color

void main(void){

    fFragClr = vec4(vInterpColor, 1.0); // by default: use object's interpolated color
}