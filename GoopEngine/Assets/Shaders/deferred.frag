
#version 460 core


layout (location=0) in vec2 fTexCoord;     // input texcoord

// OUTPUT
layout (location=0) out vec3 dFragColor;

// Additional uniforms for lighting and textures
layout (binding = 0) uniform sampler2D uPosition; // G-buffer position texture
layout (binding = 1) uniform sampler2D uAlbedo;   // G-buffer albedo texture
layout (binding = 2) uniform sampler2D uNormal;   // G-buffer normal texture

void main() {
    // Sample data from the G-buffer textures
    vec3 position = texture(uPosition, fTexCoord).xyz;
    vec3 albedo =   texture(uAlbedo, fTexCoord).xyz;
    vec3 normal =   texture(uNormal, fTexCoord).xyz;
    // Lighting calculations go here (e.g., diffuse, specular, shadow maps)

    // Set the final pixel color
    dFragColor = albedo;
}
