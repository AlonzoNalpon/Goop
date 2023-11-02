#version 460 core

layout (location=0) in vec2 vTexCoord;      // interpolated texcoordinates

layout (location=0) out vec4 fFragClr;      // output fragment color

layout (binding = 7) uniform sampler2D uFramebufferTexture; // Bind to texture unit 7
void main(void){

    fFragClr = texture(uFramebufferTexture, vTexCoord); // assign color from lookup value
    //fFragClr = vec4(vTexCoord.s, vTexCoord.t, 1.0, 1.0);
}