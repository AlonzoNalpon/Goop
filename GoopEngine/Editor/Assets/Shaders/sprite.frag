/* !
@file    sprite.vert
@author  a.nalpon@digipen.edu
@date    06/10/2023

This is the basic sprite shader for rendering sprites
*//*__________________________________________________________________________*/
#version 460 core

layout (location=0) in vec3 vInterpColor;   // interpolated color
layout (location=1) in vec2 vTexCoord;      // 

layout (location=0) out vec4 fFragClr;      // output fragment color

//uniform sampler2D uTex2d; // For task 3: struct to access image
layout (binding = 7) uniform sampler2D uTex2d; // Bind to texture unit 7
layout (location=5) uniform vec4 uTint; // to multiply with the final color
layout (location=6) uniform vec4 uClrMult; // color multiply for each model

void main(void){

    fFragClr = texture(uTex2d, vTexCoord); // assign color from lookup value
    fFragClr.rgb = mix(fFragClr.rgb, uTint.rgb, uTint.a);
    fFragClr *= uClrMult;
    //fFragClr.rgb *= fFragClr.a;  // Premultiply alpha. Leave this here in case we switch over to this method
    //fFragClr = vec4(vInterpColor, 1.0); // by default: use object's interpolated color
}