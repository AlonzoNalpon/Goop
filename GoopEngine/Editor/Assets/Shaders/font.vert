#version 460 core

layout (location = 0) in vec4 vertex; // vec2pos & vec2 tex
out vec2 texCoords;

layout (location = 0) uniform mat4 uProj;

void main()
{
  gl_Position = uProj * vec4(vertex.xy, 0.0, 1.0);
  texCoords = vertex.zw;  // xy for position, zw for texcoords
}