/* !
@file    glhelper.h
@author  a.nalpon@digipen.edu
@date    06/10/2023

This file is a basic fragment shader meant to work with my-tutorial-5.vert
shader.

tutorial 4: texture mapping with color modulation has been added. See the
different cases from the specs or glapp.cpp
*//*__________________________________________________________________________*/
#version 450 core

layout (location=1) in vec2 vTexCoord;      // Task 3: interpolated tex coordinates (by fixed-stage rasterizer)
layout (location=0) in vec3 vInterpColor;   // interpolated color

layout (location=0) out vec4 fFragClr;      // output fragment color

uniform int uTask_ID;                       // current task ID to set appropriate demo
uniform bool uModulate_Color;               // a flag to indicate if colors should be modulated
uniform float uTileSize;                    // For task 2: accumulated time

uniform sampler2D uTex2d; // For task 3: struct to access image

void main(void){
    switch(uTask_ID){
    case 1: // case 1: checker board
    case 2:
    {
        ivec2 rounded_pos = ivec2(  gl_FragCoord.x/uTileSize, 
                                    gl_FragCoord.y/uTileSize);
        // Creating alternating patterns for a checkboard look
        if (mod(rounded_pos.x + rounded_pos.y, 2) == 1){
            fFragClr = vec4(0.0, 0.68, 0.94, 1.0);
        }
        else{
            fFragClr = vec4(1.0, 0.0, 1.0, 1.0);
        }
    }
        break;
    case 3: // default texture (still repeat but texcoord not scaled)
    case 4: // repeat (with texcoord scaled by 4)
    case 5: // mirror
    case 6: // clamp
    {
        fFragClr = texture(uTex2d, vTexCoord); // assign color from lookup value
    }
        break;
    default:
        fFragClr = vec4(vInterpColor, 1.0); // by default: use object's interpolated color
        break;
  }
  if (uModulate_Color)
    fFragClr = fFragClr * vec4(vInterpColor, 1.0); // modulate colors only if uniform is true
}