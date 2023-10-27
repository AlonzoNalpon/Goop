#version 460 core

in vec2 texCoords;
out vec4 color;

//layout (location = 0) uniform mat4 uProj;
uniform sampler2D uText;
layout (location = 2) uniform vec3 uTextColor;
void main()
{
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uText, texCoords).r);
  color = vec4(uTextColor, 1.0) * sampled;
}