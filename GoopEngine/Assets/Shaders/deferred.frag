
#version 460 core

layout (location=0) out vec4 dFragColor;

layout (location=0) in vec3 fPosition;    // output fragment position
layout (location=1) in vec3 fAlbedo;      // output fragment albedo
layout (location=2) in vec3 fNormal;      // output fragment normal
layout (location=3) in vec2 fTexCoord;    // output tex coord 

layout (binding = 0) uniform sampler2D uPosition; // G-buffer position texture
layout (binding = 1) uniform sampler2D uAlbedo;   // G-buffer albedo texture
layout (binding = 2) uniform sampler2D uNormal;   // G-buffer normal texture
//layout(binding = 3) uniform sampler2D uTexCoord; // G-buffer texture coordinate texture
//layout(binding = 4) uniform sampler2D uDepth;    // G-buffer depth texture
// me no use depth or texcoord for now
layout (binding = 7) uniform sampler2D uTex2d; // Bind to texture unit 7

// Additional uniforms for lighting and textures


void main() {
    // Sample data from the G-buffer textures
    vec3 position = texture(uPosition,  fPosition).xyz;
    vec3 albedo   = texture(uAlbedo,    fTexCoord).xyz;
    vec3 normal   = texture(uNormal,    fNormal).xyz;

    // Lighting calculations go here (e.g., diffuse, specular, shadow maps)

    // Apply textures
    vec3 finalColor = albedo; // Start with albedo color
    vec4 textureColor = texture(uTex2d, texCoord);
    finalColor *= textureColor.rgb; // Apply texture color

    // Set the final pixel color
    dFragColor = vec4(finalColor, 1.0);
}
