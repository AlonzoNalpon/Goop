
#version 460 core

//layout (location=0) in vec3 vInterpColor;   // interpolated color
//layout (location=1) in vec2 vTexCoord;      // 

layout (location=0) in vec3 vFragPosition; // input fragment position
layout (location=1) in vec3 vFragAlbedo;   // input fragment albedo
layout (location=2) in vec3 vFragNormal;   // input fragment normal
layout (location=3) in vec2 vFragTexCoord; // input fragment texcoord

layout (location=0) out vec3 fPosition;    // output fragment position
layout (location=1) out vec3 fAlbedo;      // output fragment albedo
layout (location=2) out vec3 fNormal;      // output fragment normal
layout (location=3) out vec2 fTexCoord;    // output tex coord 

//uniform sampler2D uTex2d; // For task 3: struct to access image
layout (binding = 7) uniform sampler2D uTex2d; // Bind to texture unit 7
void main(void){
    fPosition = vFragPosition;    // store position
    fAlbedo   = vFragAlbedo;      // store albedo color value
    fNormal   = vFragNormal;      // store normal
    fTexCoord = vFragTexCoord;    // store texture coords
}