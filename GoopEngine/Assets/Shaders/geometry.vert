
#version 460 core
layout (location=0) in vec3 aVertexPosition;  // vertex position attribute
layout (location=1) in vec3 aVertexColor;     // vertex color attribute
layout (location=2) in vec3 aVertexNormal;    // vertex normal attribute
layout (location=3) in vec2 aTexturePosition; // texture position attribute

layout (location=0) out vec3 vPosition; // vertex position output
layout (location=1) out vec3 vAlbedo;   // vertex color output
layout (location=2) out vec3 vNormal;   // vertex normal output
layout (location=3) out vec2 vTexCoord; // texture coordinate output

// UNIFORMS GO HERE
layout (location=0) uniform vec2 uTexPos;       // the bottom left position of tex usually ranging from [0, 1]
layout (location=1) uniform vec2 uTexDims;      // the dimensions of the texture usually ranging from [0, 1]
layout (location=2) uniform mat4 uViewProjMtx;  // transformation for the model
layout (location=3) uniform mat4 uMdlMtx;       // transformation for the model
  
void main() {
  
  mat4 MVP    = uViewProjMtx * uMdlMtx;
  vPosition   = vec3(MVP * vec4(aVertexPosition, 1.0));
  vAlbedo     = aVertexColor;
  vNormal     = normalize(mat3(transpose(inverse(uMdlMtx))) * aVertexNormal);
  vTexCoord   = aTexturePosition * uTexDims + uTexPos;
}
